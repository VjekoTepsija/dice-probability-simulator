#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Dice.h"
#include "Simulator.h"
#include "RandRng.h"
#include "Mt19937Rng.h"


int main() {

	// Dice sides input

	std::cout << "Dice sides, max is 100: \n";
	int sides = 0;
	std::cin >> sides;
	if (sides < 2 || sides > 100) {
		std::cerr << "Invalid number of sides. Defaulting to 6 sides.\n";
		sides = 6;
	}

	// Dice number input

	std::cout << "Number of dice:\n";
	int dice = 0;
	std::cin >> dice;
	if (dice < 1) {
		std::cerr << "Invalid dice number.\n";
		return 1;
	}

	//Trials input

	std::cout << "Trials:\n";
	int trials = 0;
	if (!(std::cin >> trials) || trials <= 0) {
		std::cerr << "Invalid number of trials.\n";
		return 1;
	}

	//Batch input

	std::cout << "Batch size(Rounds of Trials):\n";
	int batch = 0;
	std::cin >> batch;
	if (batch < 1) {
		std::cerr << "Invalid batch size.\n";
		return 1;
	}

	// Choose RNG type

	std::cout << "1. for Rand() rng, 2. for Mersenne Twister rng\n";
	int choice = 0;
	std::cin >> choice;

	auto rng = std::unique_ptr<IRng>();

	if (choice == 1) {
		std::cout << "Using Rand() rng, choose 1 for random, or input your seed for fixed: \n";
		std::cin >> choice;
		if (choice == 1)
			rng = std::make_unique<RandRng>();
		else
			rng = std::make_unique<RandRng>(choice);
	}
	else if (choice == 2) {
		std::cout << "Using mt19937() rng, choose 1 for random, or input seed for fixed: \n";
		std::cin >> choice;
		if (choice == 1)
			rng = std::make_unique<Mt19937Rng>();
		else
			rng = std::make_unique<Mt19937Rng>(choice);
	}
	else {
		std::cerr << "Invalid choice for RNG type.\n";
		return 2;
	}

	// Create Dice and Simulator instances

	Dice d(sides, std::move(rng));
	Simulator sim(dice, trials, sides);

	// Run simulation, display and export results
	if (batch <= 1) {
		sim.run(d);
		sim.display();
		sim.InsertionSort();
		sim.displaySorted();
		sim.exportCSV("results.csv");
	}
	else {
		sim.runBatches(d, batch);
		sim.displayWelford();
	}

	return 0;
}
