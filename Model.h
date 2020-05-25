#pragma once

#include <vector>
#include "Phenotype.h"

class Model {
public:
	Model(const std::vector<std::pair<float, float>>& cities, int size);
	Model(const Model& other);
	Model(const Model&& other) noexcept;
	virtual ~Model();

	Phenotype* generate() const;
	float fitness(const Phenotype& phenotype) const;
private:
	float distance(std::pair<float, float> first, std::pair<float, float> second) const;

	int size_;
	std::vector<std::pair<float, float>> cities_;
};