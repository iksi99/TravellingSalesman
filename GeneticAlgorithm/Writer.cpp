#include "../Exceptions.h"
#include "Writer.h"

Writer::Writer(const std::string& filename)
{
	outfile_.open(filename, std::ios::out);
	if (!outfile_.is_open()) throw IOException(("Could not open file: " + filename).c_str());
}

Writer::~Writer()
{
	outfile_.close();
}

void Writer::write_fitness(int n, float max, float average)
{
	outfile_ << n << " " << max << " " << average << '\n';
}

void Writer::write_string(std::string data)
{
	outfile_ << data << '\n';
}
