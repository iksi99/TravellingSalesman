#pragma once

#include <fstream>
#include <iomanip>
#include <string>
#include <vector>


class Writer {
public:
	Writer(const std::string& filename);
	Writer(const Writer& other) = delete;
	Writer operator=(const Writer& other) = delete;
	Writer(const Writer&& other) = delete;
	~Writer();

	void write_fitness(std::pair<int, float>);
	void write_string(std::string data);

private:
	std::fstream outfile_;
};