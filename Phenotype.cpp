#include "Exceptions.h"
#include "Phenotype.h"
#include "RandomGenerator.h"
#define GENE_SWAP_PROBABILITY 0.33

Phenotype::Phenotype()
{
}

Phenotype::Phenotype(const std::vector<int>& data) : data_(data)
{
}

Phenotype::Phenotype(const Phenotype& other)
{
	for (int i = 0; i < other.data_.size(); i++) data_.push_back(other.data_[i]);
}

Phenotype::Phenotype(const Phenotype&& other)
{
	data_ = other.data_;
}

Phenotype::~Phenotype()
{
}

int Phenotype::operator[](int index)
{
	return data_[index];
}

void Phenotype::mutate(Mutation model)
{

	RandomGenerator* rand = RandomGenerator::getInstance();

	switch (model)
	{
	case UNIFORM:

		//randomly swap values around
		
		int size = data_.size();

		for (int i = 0; i < size; i++)
		{
			int other = rand->UniformInteger(0, data_.size() - 1);

			if (rand->Bernoulli(GENE_SWAP_PROBABILITY)) {
				int swap = data_[i];
				data_[i] = data_[other];
				data_[other] = swap;
			}
		}

		break;
	case INTERVAL:

		//generate random points i and j, then reverse order of section [i, j]

		int i = rand->UniformInteger(0, data_.size() - 2);
		int j = rand->UniformInteger(i+1, data_.size() - 1);

		int len = j - 1 + 1;

		for (int k = 0; k < len / 2; k++) {
			int swap = data_[i + k];
			data_[i + k] = data_[j - k];
			data_[j - k] = swap;
		}

		break;
	default:
		throw MutationException();
	}

	
}

Phenotype* Phenotype::crossover(Phenotype parent1, Phenotype parent2, Crossover model)
{
	RandomGenerator* rand = RandomGenerator::getInstance();

	std::vector<int> data;

	//helper vector for PMX
	std::vector<int> p;

	//helper vector for UPMX
	std::vector<bool> selected;

	int size;

	if (parent1.data_.size() != parent2.data_.size()) throw ParentMismatchException();
	else size = parent1.data_.size();

	switch (model)
	{
	case UNIFORM:
		//for each gene, randomly select from one of the parents

		for (int i = 0; i < size; i++) {
			if (rand->Bernoulli(0.5)) data.push_back(parent1.data_[i]);
			else data.push_back(parent2.data_[i]);
		}

		break;

	case CX:

		data.resize(size);
		for (int i = 0; i < size; i++) data[i] = -1;

		//transcription of parent 1 genes

		data[0] = parent1.data_[0];

		int i = 0;
		bool cycle = true;
		do {
			//search for location of corresponding gene from parent 2 in parent 1
			int j;
			for (j = 0; parent2.data_[i] != parent1.data_[j]; j++);

			//transcribe the gene in the child in the exact location it has in parent 1
			//this opeartion is a closed cycle: when the first iteration of the cycle is finished, end the loop
			if (data[j] == parent1.data_[j]) cycle = false;
			else data[j] == parent1.data_[j];

			i = j;

		} while (cycle);

		//all genes not yet initialized will be copied from parent 2

		for (int i = 0; i < size; i++) if (data[i] = -1) data[i] = parent2.data_[i];

		break;

	case PMX:

		//crossover points
		int xp1 = rand->UniformInteger(0, size - 2);
		int xp2 = rand->UniformInteger(xp1 + 1, size - 1);

		//initialize vectors
		data.resize(size);
		p.resize(size);
		for (int i = 0; i < size; i++) {
			data[i] = -1;
			p[i] = -1;
		}

		//copy segment from parent 1
		for (int i = xp1; i <= xp2; i++) {
			data[i] = parent1.data_[i];
			p[data[i]] = i;
		}

		//check same segment in parent 2
		for (int i = xp1; i <= xp2; i++) {

			//was the value not already copied?
			if (p[parent2.data_[i]] == -1) {
				
				bool inside;
				int value = parent1.data_[i];

				do {
					int j;

					//take value in same position in parent 1 and locate it in parent 2
					for (j = 0; parent2.data_[j] != value; j++);

					//if the value is inside, take the corresponding value from parent 1 again, then repeat loop
					if (j >= xp1 && j <= xp2) {
						inside = true;
						value = data[j];
					}
					//if the value is outside, place the ORIGINAL (at start of loop) value from parent 2 and insert into child
					else {
						inside = false;
						data[j] = parent2.data_[i];
						p[data[j]] = j;
					}

				} while (inside);
			}
		}

		//FINALLY, copy all uninitialized child values from parent 2
		for (int i = 0; i <= size; i++) {
			if (data[i] == -1) data[i] = parent2.data_[i];
		}

		break;

	case UPMX:

		//initialize vectors
		data.resize(size);
		p.resize(size);
		selected.resize(size);
		for (int i = 0; i < size; i++) {
			data[i] = -1;
			p[i] = -1;
			selected[i] = false;
		}

		//for each element in child, flip a coin to determine whether or not to copy it from parent 1
		for(int i = 0; i < size; i++)
			if (rand->Bernoulli(0.5)) {
				data[i] = parent1.data_[i];
				p[data[i]] = i;
				selected[i] = true;
			}

		//check the same randomly selected indexes in parent 2
		for (int i = 0; i < size; i++) {
			if (selected[i]) {
				//was the value not already copied?
				if (p[parent2.data_[i]] == -1) {

					bool inside;
					int value = parent1.data_[i];

					do {
						int j;

						//take value in same position in parent 1 and locate it in parent 2
						for (j = 0; parent2.data_[j] != value; j++);

						//if the value is already selected, take the corresponding value from parent 1 again, then repeat loop
						if (selected[j]) {
							inside = true;
							value = data[j];
						}
						//if the value is outside, place the ORIGINAL (at start of loop) value from parent 2 and insert into child
						else {
							inside = false;
							data[j] = parent2.data_[i];
							p[data[j]] = j;
						}

					} while (inside);
				}
			}
		}

		//FINALLY, copy all uninitialized child values from parent 2
		for (int i = 0; i <= size; i++) {
			if (data[i] == -1) data[i] = parent2.data_[i];
		}

		break;

	case NWOX:

		//crossover points
		int xp1 = rand->UniformInteger(0, size - 2);
		int xp2 = rand->UniformInteger(xp1 + 1, size - 1);

		int e = 0, i = 0;

		//copy all elements of parent 1 that aren't in the crossover region of parent 2, until there are xp1 elements in child
		while (e < xp1) {
			bool copy_ok = true;
			for (int j = xp1; j <= xp2; j++) if (parent1.data_[i] == parent2.data_[j]) copy_ok = false;
			if (copy_ok) {
				data.push_back(parent1.data_[i]);
				e++;
			}
			i++;
		}
		
		//copy crossover region from parent 2
		for (int j = xp1; j <= xp2; j++) {
			data.push_back(parent2.data_[j]);
			e++;
		}

		//copy remains of parent 1
		while (i < size) {
			bool copy_ok = true;
			for (int j = xp1; j <= xp2; j++) if (parent1.data_[i] == parent2.data_[j]) copy_ok = false;
			if (copy_ok) {
				data.push_back(parent1.data_[i]);
				e++;
			}
			i++;
		}

		break;

	case OX:

		//crossover points
		int xp1 = rand->UniformInteger(0, size - 2);
		int xp2 = rand->UniformInteger(xp1 + 1, size - 1);

		data.resize(size);
		for (int i = 0; i < size; i++) data[i] = parent1.data_[i];

		//delete all elements that exist in the crossover region of parent 2 to make 'holes'
		for (int i = 0; i < size; i++)
			for (int j = xp1; j <= xp2; j++)
				if (data[i] == parent2.data_[j]) data[i] = -1;

		//starting with the second crossover point, slide all non-hole data left so the holes form a continuous unit
		//wrapping from left to right end is allowed.

		//find the first non-hole
		int head = -1;

		//there must be at least one non-hole in the crossover region
		//otherwise, all the holes are already in the crossover region, so we don't need to do anything
		for (int i = xp2; i >= xp1; i--) {
			if (data[i] != -1) head = i;
		}

		//if there is a non-hole, we need to perform a sliding motion to align the holes with the crossover region
		if (head != -1) {
			for (int i = head; i > xp2 - size + 1; i--) {
				int j = i - 1;

				//enable proper wrapping
				int index_i = i >= 0 ? i : size + i;
				int index_j = j >= 0 ? j : size + j;

				//slide the section of non-zeros
				if (data[index_j] == -1) {
					for (int k = j; k < head; k++) {
						int l = k + 1;

						//enable proper wrapping
						int index_k = k >= 0 ? k : size + k;
						int index_l = l >= 0 ? l : size + l;

						//perform the shift
						int swap = data[index_k];
						data[index_k] = data[index_l];
						data[index_l] = swap;
					}
					head--;
				}
			}
		}

		//crossover zone is now all holes, so we write in the crossover zone of parent 2
		for (int i = xp1; i <= xp2; i++) {
			data[i] = parent2.data_[i];
		}

		break;

	default:
		throw CrossoverException();
	}

	return new Phenotype(data);
}

void Phenotype::push(int element)
{
	data_.push_back(element);
}

int Phenotype::get(int index) const
{
	return data_[index];
}
