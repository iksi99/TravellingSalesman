#pragma once

#include <vector>

enum Crossover {
	//TODO: implement various crossover strategies
};

class Gene {
public:
	Gene();
	Gene(const Gene& other);
	Gene(const Gene&& other);
	virtual ~Gene();

	int operator[](int index);

	void mutate();
	static Gene* crossover(Gene parent1, Gene parent2);

	void push();
	void get(int index) const;

private:
	std::vector<int> data_;
};