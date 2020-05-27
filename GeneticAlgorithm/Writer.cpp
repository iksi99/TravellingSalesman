#include "Writer.h"

Writer::Writer(const std::string& filename)
{
	outfile_.open(filename, std::ios::out);
}

Writer::~Writer()
{
	outfile_.close();
}

void Writer::write_fitness(std::pair<int, float> data)
{
	outfile_ << data.first << " " << data.second << '\n';
}

void Writer::write_solution(std::vector<int> data)
{
	for (int i = 0; i < (int)data.size(); i++) outfile_ << data[i] << " -> ";
	outfile_ << data[0] << '\n';
}
