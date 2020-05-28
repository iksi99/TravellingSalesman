#include <vector>

#include "pch.h"
#include "CppUnitTest.h"


#include "../Model.h"
#include "../Phenotype.h"
#include "../RandomGenerator.h"

#define MUTATION_ITERATION 5
#define NO_OF_CHILDREN 100

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TravellingSalesmanTest
{
	TEST_CLASS(MutationTest)
	{
	public:

		TEST_METHOD_INITIALIZE(PopulationInit)
		{
			population.resize(5);
			hash = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

			population[0] = new Phenotype({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 });
			population[1] = new Phenotype({ 1, 3, 5, 7, 9, 0, 2, 4, 6, 8 });
			population[2] = new Phenotype({ 6, 3, 4, 2, 8, 9, 0, 7, 1, 5 });
			population[3] = new Phenotype({ 9, 1, 5, 7, 0, 6, 3, 4, 2, 8 });
			population[4] = new Phenotype({ 5, 3, 2, 9, 4, 8, 1, 0, 7, 6 });
		}

		TEST_METHOD(UniformMutationTest)
		{
			int size = population.size();

			//run a number of mutations on the population
			for (int i = 0; i < MUTATION_ITERATION; i++)
				for (Phenotype* p : population) p->mutate(Mutation::UNIFORM);

			//count the appearances of each element
			for (Phenotype* p : population)
				for (int i = 0; i < 10; i++)
					hash[p->get(i)]++;

			//check if the genes are still valid
			for (int i : hash) Assert::AreEqual(i, size);
		}

		TEST_METHOD(IntervalMutationTest)
		{
			int size = population.size();

			//run a number of mutations on the population
			for (int i = 0; i < MUTATION_ITERATION; i++)
				for (Phenotype* p : population) p->mutate(Mutation::INTERVAL);

			//count the appearances of each element
			for (Phenotype* p : population)
				for (int i = 0; i < 10; i++)
					hash[p->get(i)]++;

			//check if the genes are still valid
			for (int i : hash) Assert::AreEqual(i, size);
		}

		TEST_METHOD_CLEANUP(PopulationDestroy)
		{
			for (Phenotype* p : population) delete p;
		}

	private:

		std::vector<Phenotype*> population;
		std::vector<int> hash;

	};

	TEST_CLASS(CrossoverTest)
	{
	public:

		TEST_METHOD_INITIALIZE(PopulationInit)
		{
			rand = RandomGenerator::getInstance();

			population.resize(5);
			children.resize(NO_OF_CHILDREN);
			hash = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

			//manually picked initial population
			population[0] = new Phenotype({ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 });
			population[1] = new Phenotype({ 1, 3, 5, 7, 9, 0, 2, 4, 6, 8 });
			population[2] = new Phenotype({ 6, 3, 4, 2, 8, 9, 0, 7, 1, 5 });
			population[3] = new Phenotype({ 9, 1, 5, 7, 0, 6, 3, 4, 2, 8 });
			population[4] = new Phenotype({ 5, 3, 2, 9, 4, 8, 1, 0, 7, 6 });
		}

		TEST_METHOD(UniformCrossoverTest)
		{
			//create a preset number of children
			for (int i = 0; i < NO_OF_CHILDREN; i++) {
				
				int parent1, parent2;

				//make sure parents are different
				do {
					parent1 = rand->UniformInteger(0, 4);
					parent2 = rand->UniformInteger(0, 4);
				} while (parent1 == parent2);

				children[i] = Phenotype::crossover(*population[parent1], *population[parent2], Crossover::UX);
			}

			//count the appearances of each element
			for (Phenotype* p : children)
				for (int i = 0; i < 10; i++)
					hash[p->get(i)]++;

			//check if the children represent proper phenotypes
			for (int i : hash) Assert::AreEqual(i, NO_OF_CHILDREN);
		}

		TEST_METHOD(CycleCrossoverTest)
		{
			//create a preset number of children
			for (int i = 0; i < NO_OF_CHILDREN; i++) {

				int parent1, parent2;

				//make sure parents are different
				do {
					parent1 = rand->UniformInteger(0, 4);
					parent2 = rand->UniformInteger(0, 4);
				} while (parent1 == parent2);

				children[i] = Phenotype::crossover(*population[parent1], *population[parent2], Crossover::CX);
			}

			//count the appearances of each element
			for (Phenotype* p : children)
				for (int i = 0; i < 10; i++)
					hash[p->get(i)]++;

			//check if the children represent proper phenotypes
			for (int i : hash) Assert::AreEqual(i, NO_OF_CHILDREN);
		}

		TEST_METHOD(PMXTest)
		{
			//create a preset number of children
			for (int i = 0; i < NO_OF_CHILDREN; i++) {

				int parent1, parent2;

				//make sure parents are different
				do {
					parent1 = rand->UniformInteger(0, 4);
					parent2 = rand->UniformInteger(0, 4);
				} while (parent1 == parent2);

				children[i] = Phenotype::crossover(*population[parent1], *population[parent2], Crossover::PMX);
			}

			//count the appearances of each element
			for (Phenotype* p : children)
				for (int i = 0; i < 10; i++)
					hash[p->get(i)]++;

			//check if the children represent proper phenotypes
			for (int i : hash) Assert::AreEqual(i, NO_OF_CHILDREN);
		}

		TEST_METHOD(UPMXTest)
		{
			//create a preset number of children
			for (int i = 0; i < NO_OF_CHILDREN; i++) {

				int parent1, parent2;

				//make sure parents are different
				do {
					parent1 = rand->UniformInteger(0, 4);
					parent2 = rand->UniformInteger(0, 4);
				} while (parent1 == parent2);

				children[i] = Phenotype::crossover(*population[parent1], *population[parent2], Crossover::UPMX);
			}

			//count the appearances of each element
			for (Phenotype* p : children)
				for (int i = 0; i < 10; i++)
					hash[p->get(i)]++;

			//check if the children represent proper phenotypes
			for (int i : hash) Assert::AreEqual(i, NO_OF_CHILDREN);
		}

		TEST_METHOD(NWOrderedCrossoverTest)
		{
			//create a preset number of children
			for (int i = 0; i < NO_OF_CHILDREN; i++) {

				int parent1, parent2;

				//make sure parents are different
				do {
					parent1 = rand->UniformInteger(0, 4);
					parent2 = rand->UniformInteger(0, 4);
				} while (parent1 == parent2);

				children[i] = Phenotype::crossover(*population[parent1], *population[parent2], Crossover::NWOX);
			}

			//count the appearances of each element
			for (Phenotype* p : children)
				for (int i = 0; i < 10; i++)
					hash[p->get(i)]++;

			//check if the children represent proper phenotypes
			for (int i : hash) Assert::AreEqual(i, NO_OF_CHILDREN);
		}

		TEST_METHOD(OrderedCrossoverTest)
		{
			//create a preset number of children
			for (int i = 0; i < NO_OF_CHILDREN; i++) {

				int parent1, parent2;

				//make sure parents are different
				do {
					parent1 = rand->UniformInteger(0, 4);
					parent2 = rand->UniformInteger(0, 4);
				} while (parent1 == parent2);

				children[i] = Phenotype::crossover(*population[parent1], *population[parent2], Crossover::OX);
			}

			//count the appearances of each element
			for (Phenotype* p : children)
				for (int i = 0; i < 10; i++)
					hash[p->get(i)]++;

			//check if the children represent proper phenotypes
			for (int i : hash) Assert::AreEqual(i, NO_OF_CHILDREN);
		}

		TEST_METHOD_CLEANUP(PopulationDestroy)
		{
			for (Phenotype* p : population) delete p;
			for (Phenotype* p : children) delete p;
		}

	private:

		std::vector<Phenotype*> population;
		std::vector<Phenotype*> children;
		std::vector<int> hash;

		RandomGenerator* rand;

	};

	TEST_CLASS(ModelTest)
	{
	public:

		TEST_METHOD_INITIALIZE(MakeModel)
		{
			model = new Model({ {0, 0}, {4, 0}, {4, 3}, {7, 7}, {16, 9} });
		}

		TEST_METHOD(GenerationTest)
		{
			population.resize(NO_OF_CHILDREN);
			hash.resize(5);

			for (int i = 0; i < NO_OF_CHILDREN; i++) {
				population[i] = model->generate();
			}

			//count the appearances of each element
			for (Phenotype* p : population)
				for (int i = 0; i < 5; i++)
					hash[p->get(i)]++;

			//check if the children represent proper phenotypes
			for (int i : hash) Assert::AreEqual(i, NO_OF_CHILDREN);
		}

		TEST_METHOD_CLEANUP(DeleteModel)
		{
			delete model;
		}

	private:
		Model* model;
		std::vector<Phenotype*> population;
		std::vector<int> hash;
	};
}