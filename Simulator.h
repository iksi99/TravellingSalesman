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
	void initialize(Model& model, int pop_size, int pool_size);

	//runs the simulation loop once
	float simulate();

	//number of times simulation loop was run
	int numberOfGenerations();

private:
	//singleton
	Simulator();
	static Simulator *instance;

	std::pair<int, int> selection(Selection strategy);

	bool initialized_;

	int generation_;
	int pop_size_;

	std::vector<Phenotype*> population_;
	std::vector<float> fitness_;
	std::vector<int> reproduction_pool_;

	Model *model_;
};