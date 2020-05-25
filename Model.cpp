#include <vector>
#include "Model.h"
#include "RandomGenerator.h"

Model::Model(const std::vector<Edge>& edges, int size) : size_(size)
{
	adjList.resize(size);

	for (const Edge& edge : edges) {
		int source = edge.source;
		int destination = edge.destination;
		int weight = edge.weight;

		adjList[source].push_back(std::make_pair(destination, weight));
		adjList[destination].push_back(std::make_pair(source, weight));
	}
}

Model::Model(const Model& other) : size_(other.size_)
{
	adjList.resize(other.adjList.size());
	for (int i = 0; i < other.adjList.size(); i++) {
		adjList[i].resize(other.adjList[i].size());
		for (int j = 0; j < other.adjList[i].size(); j++) {
			adjList[i][j] = std::make_pair(other.adjList[i][j].first, other.adjList[i][j].second);
		}
	}
}

Model::Model(const Model&& other) : size_(other.size_)
{
	adjList = other.adjList;
}

Model::~Model()
{
}

Phenotype* Model::generate() const
{
	std::vector <int> data;
	RandomGenerator* rand = RandomGenerator::getInstance();

	//generating array to shuffle
	for (int i = 0; i < size_; i++) {
		data.push_back(i);
	}

	//Fisher-Yates shuffle
	for (int i = 0; i < data.size() - 2; i++) {
		int j = rand->UniformInteger(i, data.size() - 1);

		//swap i-th and j-th element
		int swap = data[i];
		data[i] = data[j];
		data[j] = swap;
	}

	return new Phenotype(data);
}

float Model::fitness(const Phenotype& phenotype) const
{
	float fit = 0;

	for (int i = 0; i < size_ - 1; i++) {
		int current = phenotype.get(i);
		int next = phenotype.get(i + 1);
		int weight = 0;

		//find the weight for the next element
		//O(n), rewrite in maps instead of pairs for log time
		for (int j = 0; j < adjList[current].size(); j++) {
			if (adjList[i][j].first == next) weight = adjList[i][j].second;
		}

		fit += weight;
	}

	//calculate return cost
	for (int i = 0; i < adjList[size_ - 1].size(); i++)
		if (adjList[size_ - 1][i].first == phenotype.get(0)) fit += adjList[size_ - 1][i].second;

	return fit;
}
