#pragma once

#include <vector>

enum Mutation {
	//TODO: implement various mutation strategies
};

enum Crossover {
	//TODO: implement various crossover strategies
};

class Gene {
public:
	Gene();
	Gene(const std::vector<int>& data);
	Gene(const Gene& other);
	Gene(const Gene&& other);
	virtual ~Gene();

	int operator[](int index);

	void mutate();
	static Gene* crossover(Gene parent1, Gene parent2);

	void push(int element);
	int get(int index) const;

private:
	std::vector<int> data_;
};