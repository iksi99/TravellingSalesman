#pragma once

#include <vector>

enum Mutation {
	SWAP, INTERVAL
};

/*
UNIFORM - Randomly alternate between parents
CX - Cycle crossover
PMX - Partially-mapped crossover
UPMX - Uniform partially-mapped crossover
NWOX - Non-Wrapping ordered crossover
OX - Ordered crossover
*/
enum Crossover {
	UNIFORM, CX, PMX, UPMX, NWOX, OX
};

class Phenotype {
public:
	Phenotype();
	Phenotype(const std::vector<int>& data);
	Phenotype(const Phenotype& other);
	Phenotype(const Phenotype&& other);
	virtual ~Phenotype();

	int operator[](int index);

	void mutate(Mutation model);
	static Phenotype* crossover(Phenotype parent1, Phenotype parent2, Crossover model);

	void push(int element);
	int get(int index) const;

private:
	std::vector<int> data_;
};