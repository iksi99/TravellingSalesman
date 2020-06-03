#include <algorithm>
#include <cmath>
#include <vector>

#include "CppUnitTest.h"

#include "../RandomGenerator.h"
#include "../Phenotype.h"

#define SAMPLE 1000.0
#define TOLERANCE 0.1

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TravellingSalesmanTest
{
	TEST_CLASS(RandomGeneratorTest)
	{
	public:

		TEST_METHOD_INITIALIZE(InitGenerator)
		{
			rand = RandomGenerator::getInstance();
		}

		TEST_METHOD(BernoulliTest)
		{
			std::vector<float> p = { 0.0, 0.05, 0.25, 0.5, 1.0 };
			std::vector<int> expected = { 0, 50, 250, 500, 1000 };

			for (int i = 0; i < p.size(); i++) {
				std::vector<bool> results;
				results.resize(SAMPLE);

				for (int j = 0; j < SAMPLE; j++) {
					results[j] = rand->Bernoulli(p[i]);
				}

				int actual = std::count(results.begin(), results.end(), true);

				Assert::IsTrue(std::abs(expected[i] - actual) <= 2 * TOLERANCE * expected[i]);
			}
		}
		TEST_METHOD(RandomIntTest)
		{
			std::vector<int> a = { 0, 53, 632, 9876, 123456 }, b = { 10, 97, 7342, 58943, 987654 };

			for (int i = 0; i < a.size(); i++) {
				float mean_apriori = (a[i] + b[i]) / 2.0;

				float mean = 0;

				for (int j = 0; j < SAMPLE; j++) {
					mean += rand->UniformInteger(a[i], b[i]);
				}

				mean /= SAMPLE;

				Assert::IsTrue(std::fabs(mean_apriori - mean) < TOLERANCE * mean_apriori);
			}
		}

		TEST_METHOD(RandomFloatTest)
		{
			std::vector<float> a = { 0.0, 20.0, 100.0 }, b = { 1.0, 50.0, 500.0 };

			for (int i = 0; i < a.size(); i++) {
				float mean_apriori = (a[i] + b[i]) / 2.0;

				float mean = 0;

				for (int j = 0; j < SAMPLE; j++) {
					mean += rand->UniformFloat(a[i], b[i]);
				}

				mean /= SAMPLE;

				Assert::IsTrue(std::fabs(mean_apriori - mean) < TOLERANCE * mean_apriori);
			}
		}

	private:

		RandomGenerator* rand;
	};
}