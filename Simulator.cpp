#include <vector>

#include "Exceptions.h"
#include "RandomGenerator.h"
#include "Simulator.h"

#define TOURNAMENT_FACTOR 50
#define TOURNAMENT_MIN 5

Simulator* Simulator::getInstance()
{
	return instance;
}

Simulator::~Simulator()
{
	for (Phenotype* p : population_) delete p;
}

void Simulator::reset()
{
	initialized_ = false;

	generation_ = 0;
	pop_size_ = 0;

	for (Phenotype* p : population_) delete p;
	population_.clear();
	fitness_.clear();
	reproduction_pool_.clear();

	model_ = nullptr;
}

void Simulator::initialize(Model& model, int pop_size, int pool_size)
{
	reset();
	pop_size_ = pop_size;

	model_ = &model;

	//generate initial population
	for (int i = 0; i < pop_size_; i++) {
		population_.push_back(model_->generate());
	}
}

float Simulator::simulate()
{
	
	float max_fitness = 0;

	std::vector<Phenotype*> new_population;
	new_population.resize(pop_size_);

	//calculate fitness for each individual
	for (int i = 0; i < pop_size_; i++) {
		float current_fitness = model_->fitness(*population_[i]);
		fitness_.push_back(current_fitness);
		if (current_fitness > max_fitness) max_fitness = current_fitness;
	}

	//perform selection and generation of new population
	for (int i = 0; i < pop_size_; i++) {
		std::pair<int, int> mating_pair = selection(Selection::ROULETTE);
		new_population[i] = Phenotype::crossover(*population_[mating_pair.first], *population_[mating_pair.second], Crossover::NWOX);
	}

	population_ = new_population;

	//perform mutation on new population
	for (Phenotype* p : population_) {
		p->mutate(Mutation::INTERVAL);
	}

	generation_++;

	return max_fitness;
}

int Simulator::numberOfGenerations()
{
	return generation_;
}

std::pair<int, int> Simulator::selection(Selection strategy)
{
	RandomGenerator* rand = RandomGenerator::getInstance();

	int parent1 = 0, parent2 = 0;

	float total_fitness, threshold1, threshold2, s; //used in roulette wheel
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
		//can lead to low genetic diversity

		//calculate total fitness score of the population

		total_fitness = 0;

		for (float f : fitness_) {
			total_fitness += f;
		}

		threshold1 = rand->UniformFloat(0, total_fitness);
		threshold2 = rand->UniformFloat(0, total_fitness);

		//select first parent

		s = fitness_[0];
		while (s <= threshold1) {
			s += fitness_[++parent1];
		}

		//select second parent
		
		s = fitness_[0];
		while (s <= threshold2) {
			s += fitness_[++parent2];
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
			if (fitness_[participants[i]] > fitness_[winner]) winner = participants[i];

		parent1 = winner;

		participants.clear();
		participants.resize(K);

		//populate tournament participants and select second parent
		for (int i = 0; i < K; i++) participants.push_back(rand->UniformInteger(0, pop_size_ - 1));

		for (int i = 0; i < K; i++)
			if (fitness_[participants[i]] > fitness_[winner]) winner = participants[i];

		parent2 = winner;

		break;
	default:
		throw SelectionException();
	}

	return std::make_pair(parent1, parent2);
}
