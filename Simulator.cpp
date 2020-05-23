#include "Simulator.h"

Simulator* Simulator::getInstance()
{
	return instance;
}

Simulator::~Simulator()
{
	for (int i = 0; i < population_.size(); i++) delete population_[i];
}

void Simulator::reset()
{
	initialized_ = false;

	generation_ = 0;
	pop_size_ = 0;
	pool_size_ = 0;

	for (int i = 0; i < population_.size(); i++) delete population_[i];
	population_.clear();
	fitness_.clear();
	reproduction_pool_.clear();

	model_ = nullptr;
}

void Simulator::initialize(Model& model, int pop_size, int pool_size)
{
	reset();
	pop_size_ = pop_size;
	pool_size_ = pool_size;

	model_ = &model;

	//generate initial population
	for (int i = 0; i < pop_size_; i++) {
		population_.push_back(model_->generate());
	}
}

float Simulator::simulate()
{
	//TODO: simulation logic
	return 0.0f;
}

int Simulator::numberOfGenerations()
{
	return generation_;
}
