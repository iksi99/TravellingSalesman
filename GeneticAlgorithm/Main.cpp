#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "../Simulator.h"
#include "../Phenotype.h"

#include "Reader.h"
#include "Writer.h"

using namespace std;

int main(int argc, char* argv[]) {

	string inputfile;
	string outputfilef;
	string outputfiles;

	Reader* r;
	Writer* wf;
	Writer* ws;

	int population_size;
	int choose;
	int length;

	Mutation mut_strategy;
	Crossover x_strategy;
	Selection sel_strategy;

	cout << "---INPUT/OUTPUT FILES---" << endl;

	cout << "Enter input file name: ";
	cin >> inputfile;

	cout << "Enter fitness output file name: ";
	cin >> outputfilef;

	cout << "Enter solution output file name: ";
	cin >> outputfiles;

	try {
		r = new Reader(inputfile);
		wf = new Writer(outputfilef);
		ws = new Writer(outputfiles);
	}
	catch (exception e) {
		cout << e.what() << endl;

		return 1;
	}

	cout << "---SIMULATION SETTINGS---" << endl;

	cout << "Enter population size (whole number, >0): ";
	cin >> population_size;

	cout << "Enter desired length of simulation in generations (whole number, >0): ";
	cin >> length;

	cout << "Enter preferred mutation strategy (UNIFORM - 0, INTERVAL - 1): ";
	cin >> choose;

	switch (choose)
	{
	case 1:
		mut_strategy = Mutation::INTERVAL;
		break;
	default:
		mut_strategy = Mutation::UNIFORM;
	}

	cout << "Enter preferred crossover strategy (UX - 0, CX - 1, PMX - 2, UPMX - 3, NWOX - 4, OX - 5): ";
	cin >> choose;

	switch (choose)
	{
	case 1:
		x_strategy = Crossover::CX;
		break;
	case 2:
		x_strategy = Crossover::PMX;
		break;
	case 3:
		x_strategy = Crossover::UPMX;
		break;
	case 4:
		x_strategy = Crossover::NWOX;
		break;
	case 5:
		x_strategy = Crossover::OX;
		break;
	default:
		x_strategy = Crossover::UX;
	}

	cout << "Enter preferred selection strategy (RANDOM - 0, ROULETTE - 1, TOURNAMENT - 2): ";
	cin >> choose;

	switch (choose)
	{
	case 1:
		sel_strategy = Selection::ROULETTE;
		break;
	case 2:
		sel_strategy = Selection::TOURNAMENT;
		break;
	default:
		sel_strategy = Selection::RANDOM;
	}

	Simulator *sim = Simulator::getInstance();

	vector<pair<float, float>> data = r->parse();

	Model model(data);

	sim->initialize(model, population_size, mut_strategy, x_strategy, sel_strategy);

	system("CLS");
	cout << "---SIMULATION RUNNING---" << endl << "Input File: " << inputfile << endl
		<< "Output files: " << outputfilef << ", " << outputfiles << endl;
	cout << "Mutation Strategy: ";

	switch (sim->getMutationStrategy()) {
	case Mutation::UNIFORM:
		cout << "Uniform mutation" << endl;
		break;
	case Mutation::INTERVAL:
		cout << "Interval Flip mutation" << endl;
		break;
	}

	cout << "Crossover Strategy: ";

	switch (sim->getCrossoverStrategy()) {
	case Crossover::UX:
		cout << "Uniform crossover" << endl;
		break;
	case Crossover::CX:
		cout << "Cycle crossover" << endl;
		break;
	case Crossover::PMX:
		cout << "Partially Mapped Crossover" << endl;
		break;
	case Crossover::UPMX:
		cout << "Uniform Partially Mapped Crossover" << endl;
		break;
	case Crossover::NWOX:
		cout << "Non-Wrapping Ordered Crossover" << endl;
		break;
	case Crossover::OX:
		cout << "Ordered Crossover" << endl;
		break;
	}

	cout << "Selection Strategy: ";

	switch (sim->getSelectionStrategy()) {
	case Selection::RANDOM:
		cout << "Random Selection" << endl;
		break;
	case Selection::ROULETTE:
		cout << "Roulette Selection" << endl;
		break;
	case Selection::TOURNAMENT:
		cout << "Tournament Selection" << endl;
		break;
	}

	cout << "Population size: " << population_size << endl;

	for (int i = 0; i < length; i++) {
		string bests;
		float bestf, avgf;
		cout << "Generation " << i + 1 << " of " << length << "." << endl;

		bestf = sim->simulate();
		bests = sim->findCurrentBest();
		avgf = sim->averageFitness();

		cout << "Current best fitness: " << bestf << endl;
		cout << "Current average fitness: " << avgf << endl;
		cout << "Current best route: " << bests << endl;

		wf->write_fitness(make_pair(i + 1, bestf));
		ws->write_string("Generation " + to_string(i + 1) + ": " + bests);
	}

	cout << "---SIMULATION COMPLETE---" << endl;
	cout << "Results have been logged in output files by generation." << endl;

	return 0;
}