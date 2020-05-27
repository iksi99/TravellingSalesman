#pragma once

#include <fstream>
#include <string>
#include <vector>

class Reader {
public:
	Reader(const std::string& filename);
	Reader(const Reader& other) = delete;
	Reader operator=(const Reader& other) = delete;
	Reader(const Reader&& other) = delete;
	~Reader();

	std::vector<std::pair<float, float>> parse();
private:
	std::fstream infile_;

	int size_;
	std::vector<std::pair<float, float>> data_;
};