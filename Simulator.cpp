#include <algorithm>
#include <cfloat>
#include <cmath>
#include <vector>

#include "Exceptions.h"
#include "Phenotype.h"
#include "RandomGenerator.h"
#include "Simulator.h"

#define TOURNAMENT_FACTOR 50
#define TOURNAMENT_MIN 5
#define MUTATION_CHANCE 0.01

Simulator* Simulator::getInstance()
{
	return &instance;
}

Simulator::~Simulator()
{
	for (Phenotype* p : new_population_) delete p;
}

void Simulator::reset()
{
	initialized_ = false;

	generation_ = 0;
	pop_size_ = 0;

	for (Phenotype* p : population_) delete p;
	population_.clear();
	new_population_.clear();
	fitness_.clear();
	reproduction_pool_.clear();

	model_ = nullptr;
}

void Simulator::initialize(Model& model, int pop_size, Mutation mut_strategy, Crossover x_strategy, Selection sel_strategy)
{
	reset();
	pop_size_ = pop_size;

	mut_strategy_ = mut_strategy;
	x_strategy_ = x_strategy;
	sel_strategy_ = sel_strategy;

	model_ = &model;

	//generate initial population
	new_population_.resize(pop_size);
	for (int i = 0; i < pop_size_; i++) {
		new_population_[i] = model_->generate();
	}
	fitness_.resize(pop_size);

	initialized_ = true;
}

float Simulator::simulate()
{
	RandomGenerator* rand = RandomGenerator::getInstance();

	//delete old population to prevent a memory leak
	for (Phenotype* p : population_) {
		delete p;
		p = nullptr;
	}

	population_ = new_population_;
	
	float best_fitness = FLT_MAX;
	int elite = 0;

	//calculate fitness for each individual
	for (int i = 0; i < pop_size_; i++) {
		float current_fitness = model_->fitness(*population_[i]);
		fitness_[i] = current_fitness;
		if (current_fitness < best_fitness) {
			best_fitness = current_fitness;
			elite = i;
		}
	}

	//perform selection and generation of new population (empty spot left for elitism)
	for (int i = 1; i < pop_size_; i++) {
		std::pair<int, int> mating_pair = selection(sel_strategy_);
		new_population_[i] = Phenotype::crossover(*population_[mating_pair.first], *population_[mating_pair.second], x_strategy_);
	}

	//perform mutation on new population
	for (int i = 1; i < pop_size_; i++) {
		if (rand->Bernoulli(MUTATION_CHANCE)) new_population_[i]->mutate(mut_strategy_);
	}

	//by the principle of elitism, the previous best solution gets to stay in the gene pool
	new_population_[0] = new Phenotype(*population_[elite]);

	generation_++;

	return best_fitness;
}

std::vector<std::string> Simulator::getBestSolutions(int n) const
{
	//can return more than one good solution
	//inefficient on large genomes
	//O(nlogn)

	std::vector<std::pair<Phenotype*, float>> solutions;
	std::vector<std::string> returnValue;

	solutions.resize(pop_size_);
	returnValue.resize(n);

	//custom comparison condition for sorting
	struct {
		bool operator()(std::pair<Phenotype*, float> a, std::pair<Phenotype*, float> b) const {
			return a.second < b.second;
		}
	} pairLess;

	//make pairs for easy sorting
	for (int i = 0; i < pop_size_; i++) {
		solutions[i].first = population_[i];
		solutions[i].second = fitness_[i];
	}

	std::sort(solutions.begin(), solutions.end(), pairLess);

	for (int i = 0; i < n; i++)
	{
		returnValue[i] = solutions[i].first->toString();
	}

	return returnValue;
}

std::string Simulator::findCurrentBest() const
{
	//only returns the absolute best solution
	//more efficient because it doesn't require sorting the array
	//O(n)

	int best = 0;

	for (int i = 0; i < pop_size_; i++) {
		if (fitness_[i] < fitness_[best]) best = i;
	}

	return population_[best]->toString();
}

float Simulator::averageFitness() const
{
	float avg = 0;
	for (int i = 0; i < pop_size_; i++) {
		avg += fitness_[i];
	}
	avg = avg / pop_size_;
	return avg;
}

Mutation Simulator::getMutationStrategy()
{
	return mut_strategy_;
}

void Simulator::setMutationStrategy(Mutation mut_strategy)
{
	mut_strategy_ = mut_strategy;
}

Crossover Simulator::getCrossoverStrategy()
{
	return x_strategy_;
}

void Simulator::setCrossoverStrategy(Crossover x_strategy)
{
	x_strategy_ = x_strategy;
}

Selection Simulator::getSelectionStrategy()
{
	return sel_strategy_;
}

void Simulator::setSelectionStrategy(Selection sel_strategy)
{
	sel_strategy_ = sel_strategy;
}

int Simulator::numberOfGenerations()
{
	return generation_;
}

std::pair<int, int> Simulator::selection(Selection strategy)
{
	RandomGenerator* rand = RandomGenerator::getInstance();

	int parent1 = 0, parent2 = 0;

	//used in roulette wheel
	std::vector<float> normalized_fitness;
	float total_fitness, threshold1, threshold2, s, min_fitness, max_fitness, width;

	int winner, K; //used in tournament

	//helper vector for tournament selection
	std::vector<int> participants;

	switch (strategy)
	{
	case Selection::RANDOM:

		//pick two parents at random
		//low selection pressure, not recommended

		parent1 = rand->UniformInteger(0, pop_size_ - 1);
		parent2 = rand->UniformInteger(0, pop_size_ - 1);

		break;

	case Selection::ROULETTE:

		//assign slots on a roulette wheel to the population proportional to their fitness
		//selection according to absolute fitness, the fittest individuals reproduce the most

		//we need to normalize fitnesses first so that the algorithm works
		normalized_fitness.resize(pop_size_);

		//find minimum and maximum fitness in population to determine the normalization interval
		min_fitness = *std::min_element(fitness_.begin(), fitness_.end());
		max_fitness = *std::max_element(fitness_.begin(), fitness_.end());
		width = max_fitness - min_fitness;

		//normalize, but leave a certain probability for even minimum fitness elements to reproduce to ensure genetic variance
		//since fitness is distance and we want to minimize, lower fitness is actually better
		//however, the roulette algorithm operates on a higher is better principle, so we will need to flip the values
		for (int i = 0; i < pop_size_; i++) {
			normalized_fitness[i] = width - (fitness_[i] - min_fitness);
			normalized_fitness[i] = std::pow(normalized_fitness[i], 2);
		}

		//calculate total fitness score of the population

		total_fitness = 0;

		for (float f : normalized_fitness) {
			total_fitness += f;
		}

		threshold1 = rand->UniformFloat(0, total_fitness);
		threshold2 = rand->UniformFloat(0, total_fitness);

		//select first parent

		s = normalized_fitness[0];
		while (s <= threshold1) {
			s += normalized_fitness[++parent1];
		}

		//select second parent
		
		s = normalized_fitness[0];
		while (s <= threshold2) {
			s += normalized_fitness[++parent2];
		}

		break;

	case Selection::TOURNAMENT:

		//randomly select K individuals and choose the best among them as a parent

		//calculate the K factor based on population size
		//ensure it has at least a miminum size

		winner = 0;
		K = pop_size_ / TOURNAMENT_FACTOR;
		K = K < TOURNAMENT_MIN ? TOURNAMENT_MIN : K;

		participants.resize(K);

		//populate tournament participants and select first parent
		for (int i = 0; i < K; i++) participants.push_back(rand->UniformInteger(0, pop_size_ - 1));

		for (int i = 0; i < K; i++)
			if (fitness_[participants[i]] < fitness_[winner]) winner = participants[i];

		parent1 = winner;

		participants.clear();
		participants.resize(K);

		//populate tournament participants and select second parent
		for (int i = 0; i < K; i++) participants.push_back(rand->UniformInteger(0, pop_size_ - 1));

		for (int i = 0; i < K; i++)
			if (fitness_[participants[i]] < fitness_[winner]) winner = participants[i];

		parent2 = winner;

		break;
	default:
		throw SelectionException();
	}

	return std::make_pair(parent1, parent2);
}

Simulator Simulator::instance;