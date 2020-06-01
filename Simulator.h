#pragma once

#include <vector>
#include "Phenotype.h"
#include "Model.h"

enum class Selection {
	RANDOM, ROULETTE, TOURNAMENT
};

class Simulator {
public:
	//singleton
	static Simulator* getInstance();
	~Simulator();

	void reset();
	void initialize(Model& model, int pop_size, Mutation mut_strategy, Crossover x_strategy, Selection sel_strategy);

	//runs the simulation loop once
	float simulate();

	//functions for retrieving solutions
	std::vector<std::pair<std::string, float>> getNBest(int n) const;
	std::string getCurrentBest() const;

	//functions for retrieving statistical data about the population
	float averageFitness() const;
	float fitnessStDev() const;

	//getters and setters
	Mutation getMutationStrategy();
	void setMutationStrategy(Mutation mut_strategy);

	Crossover getCrossoverStrategy();
	void setCrossoverStrategy(Crossover x_strategy);

	Selection getSelectionStrategy();
	void setSelectionStrategy(Selection sel_strategy);

	//number of times simulation loop was run
	int numberOfGenerations();

private:
	//singleton
	Simulator() {}
	static Simulator instance;

	std::pair<int, int> selection(Selection strategy);

	bool initialized_;

	int generation_;
	int pop_size_;

	std::vector<Phenotype*> population_;
	std::vector<Phenotype*> new_population_;
	std::vector<float> fitness_;
	std::vector<int> reproduction_pool_;

	Mutation mut_strategy_;
	Crossover x_strategy_;
	Selection sel_strategy_;

	Model *model_;
};