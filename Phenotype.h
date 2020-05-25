#pragma once

#include <vector>

enum class Mutation {
	UNIFORM, INTERVAL
};

/*
UNIFORM - Randomly alternate between parents
CX - Cycle crossover
PMX - Partially-mapped crossover
UPMX - Uniform partially-mapped crossover
NWOX - Non-Wrapping ordered crossover
OX - Ordered crossover
*/
enum class Crossover {
	UX, CX, PMX, UPMX, NWOX, OX
};

class Phenotype {
public:
	Phenotype();
	Phenotype(const std::vector<int>& data);
	Phenotype(const Phenotype& other);
	Phenotype(const Phenotype&& other) noexcept;
	virtual ~Phenotype();

	int operator[](int index);

	void mutate(Mutation model);
	static Phenotype* crossover(Phenotype parent1, Phenotype parent2, Crossover model);

	void push(int element);
	int get(int index) const;

private:
	std::vector<int> data_;
};