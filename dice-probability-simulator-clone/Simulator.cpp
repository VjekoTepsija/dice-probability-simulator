#include "Simulator.h"
#include <fstream>
#include <iostream>
#include <iomanip>

Simulator::Simulator(int dice, int trials, int sides)
	: dice(dice), trials(trials), sides(sides) {
	faceCount.resize(sides + 1, 0);
	this->probability = 1.0 / sides;
}

long long Simulator::totalRolls() const {
	return static_cast<long long>(dice) * trials;
}

void Simulator::run(const Dice& d) {
	for (int i = 0; i < trials; i++) {
		for (int j = 0; j < dice; j++) {
			++faceCount[d.roll()];
		}
	}
}

void Simulator::display() const {
	std::cout << "\nFace  Count  Probability\n";

	double ab_error = 0;
	double rel_error = 0;

	for (int face = 1; face <= sides; face++) {
		double p = static_cast<double>(faceCount[face]) / totalRolls();
		ab_error = std::abs(p - probability);
		rel_error = std::abs(p - probability) / probability * 100;
		std::cout << face << " , " << faceCount[face] << " , " << p << " Absolute Error:  " << ab_error
			<< " , " << "Releative Error: " << rel_error << "%\n";
	}
}

void Simulator::exportCSV(const std::string& filename) const {
	std::ofstream file(filename);
	if (!file) {
		std::cerr << "Failed to open results file.\n";
		return;
	}

	double ab_error = 0;
	double rel_error = 0;
	file << "Face,Count,Probability,Absolute Error,Rel. Error(%)\n";

	for (int face = 1; face <= sides; face++) {
		double p = static_cast<double>(faceCount[face]) / totalRolls();
		ab_error = std::abs(p - probability);
		rel_error = std::abs(p - probability) / probability * 100;
		file << face << "," << faceCount[face] << "," << p << "," << ab_error
			<< ","<< rel_error << "\n";
	}

	file.close();
	std::cout << "Results exported to " << filename << "\n";
}