#include "Gene.h"
#include "RandomGenerator.h"

Gene::Gene()
{
}

Gene::Gene(const std::vector<int>& data) : data_(data)
{
}

Gene::Gene(const Gene& other)
{
	for (int i = 0; i < other.data_.size(); i++) data_.push_back(other.data_[i]);
}

Gene::Gene(const Gene&& other)
{
	data_ = other.data_;
}

Gene::~Gene()
{
}

int Gene::operator[](int index)
{
	return data_[index];
}

void Gene::mutate()
{
	//generate random points i and j, then reverse order of section [i, j]

	RandomGenerator *rand = RandomGenerator::getInstance();

	int i = rand->UniformInteger(0, data_.size() - 2);
	int j = rand->UniformInteger(i, data_.size() - 1);

	int len = j - 1 + 1;

	for (int k = 0; k < len / 2; k++) {
		int swap = data_[i + k];
		data_[i + k] = data_[j - k];
		data_[j - k] = swap;
	}
}

Gene* Gene::crossover(Gene parent1, Gene parent2)
{
	//TODO: crossover
	return nullptr;
}

void Gene::push(int element)
{
	data_.push_back(element);
}

int Gene::get(int index) const
{
	return data_[index];
}
