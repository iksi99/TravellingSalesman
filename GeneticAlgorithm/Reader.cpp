#include "../Exceptions.h"
#include "Reader.h"

Reader::Reader(const std::string& filename)
{
	infile_.open(filename, std::ios::in);

	if (!infile_.is_open()) throw ("Could not open file: " + filename).c_str();

	std::string current = "";
	std::string junk = ""; //for useless data

	size_ = 0;

	//clear the header section in preparation for data parsing
	while (current != "NODE_COORD_SECTION") {
		infile_ >> current;
		if (current == "DIMENSION") {
			infile_ >> junk; //get rid of the semicolon
			infile_ >> current;

			size_ = std::stoi(current);
			data_.resize(size_);
		}
	}
}

Reader::~Reader()
{
	infile_.close();
}

std::vector<std::pair<float, float>> Reader::parse()
{
	//the constructor has already parsed the header
	//here, we only parse the points

	std::string current = "";

	while (!infile_.eof()) {
		int i;
		float first, second;

		infile_ >> current;
		if (current == "EOF") break;
		else i = std::stoi(current) - 1;

		infile_ >> current;
		first = std::stof(current);

		infile_ >> current;
		second = std::stof(current);

		data_[i] = std::make_pair(first, second);
	}

	return data_;
}