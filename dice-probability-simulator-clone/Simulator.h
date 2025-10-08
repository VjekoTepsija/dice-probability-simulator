#pragma once
#include <vector>
#include <string>
#include "Dice.h"

class Simulator {
private:
	int dice;
	int trials;
	int sides;
	double probability;

	std::vector<long long> faceCount;
	std::vector<std::pair<int, long long>> sortedFaces;

public:
	Simulator(int dice, int trials, int sides);
	void run(const Dice& d);
	void display() const;
	void exportCSV(const std::string& filename) const;
	void displaySorted() const;
	void InsertionSort();

private:
	long long totalRolls() const;
};