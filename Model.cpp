#include <cmath>
#include <vector>
#include "Model.h"
#include "RandomGenerator.h"


Model::Model(const std::vector<std::pair<float, float>>& cities, int size) : size_(size)
{
	for (std::pair<float, float> p : cities) cities_.push_back(p);
}

Model::Model(const Model& other) : size_(other.size_)
{
	for (std::pair<float, float> p : other.cities_) cities_.push_back(p);
}

Model::Model(const Model&& other) noexcept : size_(other.size_)
{
	cities_ = other.cities_;
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
	for (int i = 0; i < (int) data.size() - 2; i++) {
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
		float dist = distance(cities_[current], cities_[next]);

		fit += dist;
	}

	//calculate return cost
	fit += distance(cities_[phenotype.get(size_ - 1)], cities_[phenotype.get(0)]);

	return fit;
}

float Model::distance(std::pair<float, float> first, std::pair<float, float> second) const
{
	return sqrt(pow(first.first - second.first, 2) + pow(first.second - second.second, 2));
}
