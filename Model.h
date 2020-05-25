#pragma once

#include <vector>
#include "Phenotype.h"

struct Edge {
	int source, destination, weight;
};

typedef std::pair<int, int> Pair;

class Model {
public:
	Model(const std::vector<Edge>& edges, int size);
	Model(const Model& other);
	Model(const Model&& other);
	virtual ~Model();

	Phenotype* generate() const;
	float fitness(const Phenotype& phenotype) const;
private:
	int size_;
	std::vector<std::vector<Pair>> adjList;
};