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

struct ParentMismatchException : public std::exception {
	const char* what() const throw() {
		return "Parent length mismatch!";
	}
};