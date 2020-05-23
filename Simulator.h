#pragma once

#include <vector>
#include "Gene.h"
#include "Model.h"

class Simulator {
public:
	//singleton
	static Simulator* getInstance();
	~Simulator();

	void reset();
	void initialize();

	//runs the simulation loop once
	float simulate();

	//number of times simulation loop was run
	int numberOfGenerations();

private:
	//singleton
	Simulator();
	static Simulator *instance;
	bool initialized_;

	int generation_;
	int pop_size_;
	int pool_size;

	std::vector<Gene> population_;
	std::vector<float> fitness_;
	std::vector<int> reproduction_pool_;

	Model *model_;
};