#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "../Exceptions.h"
#include "../Simulator.h"
#include "../Phenotype.h"

#include "Reader.h"
#include "Writer.h"

using namespace std;

void generatePlotScripts(string filename, string plotname) {

	string plt = plotname.substr(0, plotname.find('.')) + "_fit";

	fstream python, matlab;
	python.open("output/" + plt + ".py", ios::out);
	matlab.open("output/" + plt + ".m", ios::out);

	if (!python.is_open() || !matlab.is_open()) {
		throw IOException("Could not create plot scripts. You must write your own plot scripts manually.");
		return;
	}

	//generate python script
	//requires matplotlib and numpy

	python << "import matplotlib.pyplot as plt" << '\n';
	python << "import numpy as np" << '\n';

	python << endl;

	python << "data = np.loadtxt('" << filename << "')" << '\n';
	python << "data = np.transpose(data)" << '\n';

	python << endl;

	python << "plt.figure(1)" << '\n';
	python << "plt.plot(data[0, :], data[1, :], 'r')" << '\n';
	python << "plt.plot(data[0, :], data[2, :], 'b')" << '\n';
	python << "plt.legend(['Fitness of best solution', 'Average population fitness'])" << '\n';
	python << "plt.savefig('plots/" << plt << ".png')" << '\n';

	python << endl;

	python << "plt.figure(2)" << '\n';
	python << "plt.loglog(data[0, :], data[1, :], 'r')" << '\n';
	python << "plt.loglog(data[0, :], data[2, :], 'b')" << '\n';
	python << "plt.legend(['Fitness of best solution', 'Average population fitness'])" << '\n';
	python << "plt.savefig('plots/" << plt << "_log.png')" << '\n';
	
	python.close();

	//generate MATLAB script

	matlab << "a = importdata('" << filename << "');" << '\n';
	matlab << "b = a.data;" << '\n';


	matlab << endl;

	matlab << "generation = b(:, 1);" << '\n';
	matlab << "fitness = b(:, 2);" << '\n';
	matlab << "avgfitness = b(:, 3);" << '\n';

	matlab << endl;

	matlab << "figure(1)" << '\n';
	matlab << "plot(generation, fitness, 'r', generation, avgfitness, 'b')" << '\n';
	matlab << "xlim([0 max(generation)])" << '\n';
	matlab << "legend('Fitness of best solution','Average population fitness')" << '\n';
	matlab << "saveas(1, 'plots/" << plt << ".png')" << '\n';

	matlab << endl;

	matlab << "figure(2)" << '\n';
	matlab << "loglog(generation, fitness, 'r', generation, avgfitness, 'b')" << '\n';
	matlab << "xlim([0 max(generation)])" << '\n';
	matlab << "legend('Fitness of best solution','Average population fitness')" << '\n';
	matlab << "saveas(2, 'plots/" << plt << "_log.png')" << '\n';

	cout << "Plotting scripts successfully generated in output folder." << endl;
}

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
		r = new Reader("input/" + inputfile);
		wf = new Writer("output/" + outputfilef);
		ws = new Writer("output/" + outputfiles);
	}
	catch (exception e) {

		cout << e.what() << endl;

		system("PAUSE");

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

	Simulator* sim = Simulator::getInstance();

	vector<pair<float, float>> data = r->parse();

	delete r;
	r = nullptr;

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

	vector<string> solution_history;
	solution_history.resize(length);

	wf->write_string("# num_gen, best, avg");

	for (int i = 0; i < length; i++) {
		string bests;
		float bestf, avgf, stdevf;
		cout << "Generation " << i + 1 << " of " << length << "." << endl;

		bestf = sim->simulate();
		bests = sim->getCurrentBest();
		avgf = sim->averageFitness();
		stdevf = sim->fitnessStDev();

		cout << "Current best fitness: " << bestf << endl;
		cout << "Current average fitness: " << avgf << endl;
		cout << "Current standard deviation of fitness:" << stdevf << endl;

		wf->write_fitness(i + 1, bestf, avgf);
		solution_history[i] = bests;
	}

	ws->write_string("---MOST OPTIMAL SOLUTION---");
	ws->write_string(sim->getCurrentBest());

	ws->new_line();

	ws->write_string("---OTHER GOOD SOLUTIONS FROM FINAL GENERATION--- ");

	vector<pair<string, float>> winners = sim->getNBest(10);

	for (pair<string, float>& w : winners) {
		ws->write_string("Fitness: " + to_string(w.second));
		ws->write_string("Tour: " + w.first);
	}

	ws->new_line();

	ws->write_string("---BEST SOLUTION HISTORY---");
	for (int i = 0; i < (int)solution_history.size(); i++) ws->write_string("Generation " + to_string(i + 1) + ": " + solution_history[i]);

	cout << "---SIMULATION COMPLETE---" << endl;
	cout << "Results have been logged in output files by generation." << endl;

	delete wf;
	delete ws;

	try {
		generatePlotScripts(outputfilef, outputfilef);
	}
	catch (exception e){
		cout << e.what() << endl;
	}

	system("PAUSE");

	return 0;
}