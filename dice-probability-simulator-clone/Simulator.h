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

	long long n_batches = 0;
	std::vector<double> batch_prob;
	std::vector<double> ba_prob_sq;

public:
	Simulator(int dice, int trials, int sides);

	void run(const Dice& d);
	void runBatches(const Dice& d, int batches);

	void display() const;
	void displayWelford() const;

	void exportCSV(const std::string& filename) const;
	void displaySorted() const;

	void InsertionSort();
	void clearFaceCount();

private:
	long long totalRolls() const;
};