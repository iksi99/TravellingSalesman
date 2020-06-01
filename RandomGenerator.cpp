#include "RandomGenerator.h"

RandomGenerator::RandomGenerator() {
	random.seed(std::time(0));
}

RandomGenerator* RandomGenerator::getInstance()
{
	return &instance;
}

bool RandomGenerator::Bernoulli(float probability)
{
	std::bernoulli_distribution dist(probability);

	return dist(random);
}

int RandomGenerator::UniformInteger(int from, int to)
{
	std::uniform_int_distribution<int> dist(from, to);

	return dist(random);
}

float RandomGenerator::UniformFloat(float from, float to)
{
	std::uniform_real_distribution<float> dist(from, to);

	return dist(random);
}

double RandomGenerator::UniformDouble(double from, double to)
{
	std::uniform_real_distribution<double> dist(from, to);

	return dist(random);
}

RandomGenerator RandomGenerator::instance;