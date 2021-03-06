#pragma once

#include <exception>

struct MutationException : public std::exception {
	const char* what() const throw() {
		return "Invalid mutation model.";
	}
};

struct CrossoverException : public std::exception {
	const char* what() const throw() {
		return "Invalid crossover model.";
	}
};

struct SelectionException : public std::exception {
	const char* what() const throw() {
		return "Invalid selection strategy.";
	}
};

struct ParentMismatchException : public std::exception {
	const char* what() const throw() {
		return "Parent length mismatch!";
	}
};

struct SimulatorUninitializedException : public std::exception {
	const char* what() const throw() {
		return "Attempting to use uninitialized simulator";
	}
};

class IOException : public std::exception {
public:
	IOException(const char* message) : exception(message) {}
};