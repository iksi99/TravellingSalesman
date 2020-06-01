#pragma once

#include <ctime>
#include <random>

class RandomGenerator {
public:
	static RandomGenerator* getInstance();
	~RandomGenerator() {}

	bool Bernoulli(float probability);
	int UniformInteger(int from, int to);
	float UniformFloat(float from, float to);
	double UniformDouble(double from, double to);
private:
	RandomGenerator();
	static RandomGenerator instance;

	std::default_random_engine random;
};