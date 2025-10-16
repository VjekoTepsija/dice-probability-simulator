#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Dice.h"
#include "Simulator.h"
#include "RandRng.h"
#include "Mt19937Rng.h"

#include <chrono>

void runSim(Simulator& sim, Dice& d, int batches) {
	if (batches == 1) {
		sim.run(d);
		sim.display();
		sim.InsertionSort();
		sim.displaySorted();

		int export_choice = 0;
		std::cout << "Enter export choice (0 for no export, 1 for export): ";
		std::cin >> export_choice;
		if (export_choice == 1) {
			sim.exportCSV("Results.csv");
		}
	}
	else if (batches > 1) {
		sim.runBatches(d, batches);
		sim.displayWelford();
	}
}
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

	Simulator sim(dice, trials, sides);
	auto rng = std::unique_ptr<IRng>();

	std::cout << "1. RandRng(Random)\n2. RandRng(Fixed)\n3. Mt19937Rng(Random)\n4.Mt19937Rng(Fixed)\n5. Mt19937Rng Parallel Threads.\nChoose: \n";
	int choice = 0;
	std::cin >> choice;

	switch (choice) {
	case 1: {
		rng = std::make_unique<RandRng>();
		Dice d(sides, std::move(rng));
		runSim(sim, d, batch);
		break;
	}
	case 2: {
		std::cout << "Input seed:\n";
		int seed;
		std::cin >> seed;
		rng = std::make_unique<RandRng>(seed);
		Dice d(sides, std::move(rng));
		runSim(sim, d, batch);
		break;
	}
	case 3: {
		rng = std::make_unique<Mt19937Rng>();
		Dice d(sides, std::move(rng));
		runSim(sim, d, batch);
		break;
	}
	case 4: {
		std::cout << "Input seed:\n";
		int seed;
		std::cin >> seed;
		rng = std::make_unique<Mt19937Rng>(seed);
		Dice d(sides, std::move(rng));
		runSim(sim, d, batch);
		break;
	}
	case 5: {
		int threads = 0;
		std::cout << "Number of threads:\n";
		std::cin >> threads;
		if (threads < 1) {
			std::cerr << "Invalid number of threads.\n";
			return 1;
		}
		auto clock = std::chrono::steady_clock::now();
		sim.runBatchesParallel(batch, threads);
		auto clock_end = std::chrono::steady_clock::now();
		sim.displayWelford();

		auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(clock_end - clock).count();
		std::cout << "Elapsed time: " << elapsed << " ms\n";
		return 0;
	}
	default:
		std::cerr << "Invalid choice.\n";
		return 1;
	}
	return 0;
}
