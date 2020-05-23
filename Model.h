#pragma once

#include <vector>
#include "Gene.h"

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

	Gene* generate() const;
	float fitness(const Gene& gene) const;
private:
	int size_;
	std::vector<std::vector<Pair>> adjList;
};