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
	std::vector<double> mean;
	std::vector<double> m2;

public:
	Simulator(int dice, int trials, int sides);

	void run(const Dice& d);
	void runBatches(const Dice& d, int batches);
	void runBatchesParallel(int total_batches, int num_threads);

	void display() const;
	void displayWelford() const;
	void displaySorted() const;

	void InsertionSort();

	void exportCSV(const std::string& filename) const;

private:
	struct WelfordAcc {
		long long n = 0;
		std::vector<double> mean;
		std::vector<double> m2;
	};
	WelfordAcc Worker(int batches, int sides, int dice, int trials);

	//helper functions
	long long totalRolls() const;
	void clearFaceCount();
	void rollCounts(const Dice& d, int totalRolls, std::vector<int>& counts);

	void resetAcc(WelfordAcc& acc, int sides_);
	void resetFinalStats();

	void welfordUpdate(
		std::vector<double>& mean,
		std::vector<double>& m2,
		long long& n,
		const std::vector<int>& counts,
		int sides,
		int totalInBatch);
};