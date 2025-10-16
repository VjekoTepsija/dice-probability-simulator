#include "Simulator.h"
#include "Mt19937Rng.h"
#include "RandRng.h"

#include <fstream>
#include <iostream>
#include <cmath>
#include <thread>
#include <algorithm>

Simulator::Simulator(int dice, int trials, int sides)
	: dice(dice), trials(trials), sides(sides) {
	faceCount.resize(sides + 1, 0);
	this->probability = 1.0 / sides;

	resetFinalStats();
}

//Helper functions
void Simulator::rollCounts(const Dice& d, int totalRolls, std::vector<int>& counts) {
	std::fill(counts.begin(), counts.end(), 0);
	for (int t = 0; t < totalRolls; ++t) {
		const int face = d.roll();
		++counts[face];
	}
}
long long Simulator::totalRolls() const {
	return static_cast<long long>(dice) * trials;
}
void Simulator::clearFaceCount() {
	std::fill(faceCount.begin(), faceCount.end(), 0);
}

void Simulator::welfordUpdate(std::vector<double>& mean,
	std::vector<double>& m2,
	long long& n,
	const std::vector<int>& counts,
	int sides,
	int totalInBatch) {
	++n; // one batch added
	const double denom = static_cast<double>(totalInBatch);
	for (int f = 1; f <= sides; ++f) {
		const double p = static_cast<double>(counts[f]) / denom;
		const double delta = p - mean[f];
		mean[f] += delta / n;
		const double delta2 = p - mean[f];
		m2[f] += delta * delta2;
	}
}

void Simulator::resetAcc(WelfordAcc& acc, int sides_) {
	acc.n = 0;
	acc.mean.assign(sides_ + 1, 0.0);
	acc.m2.assign(sides_ + 1, 0.0);
}
void Simulator::resetFinalStats() {
	n_batches = 0;
	mean.assign(sides + 1, 0.0);
	m2.assign(sides + 1, 0.0);
}

//Struct and functions for Parallel Threads
struct MergeData {
	long long n;
	double mean;
	double m2;
};
static MergeData merge(long long n_a, double mean_a, double m2_a,
	long long n_b, double mean_b, double m2_b)
{
	if (n_a == 0 && n_b == 0) return { 0, 0.0, 0.0 };
	if (n_a == 0) return { n_b, mean_b, m2_b };
	if (n_b == 0) return { n_a, mean_a, m2_a };

	const double delta = mean_b - mean_a;
	const long long n = n_a + n_b;

	const double w_a = static_cast<double>(n_a) / n;
	const double w_b = static_cast<double>(n_b) / n;

	// Welford’s parallel variance merge formula
	const double mean = w_a * mean_a + w_b * mean_b;
	const double m2 = m2_a + m2_b
		+ delta * delta * (static_cast<double>(n_a) * static_cast<double>(n_b) / n);

	return { n, mean, m2 };
}

Simulator::WelfordAcc Simulator::Worker(int batches, int sides, int dice, int trials) {
	auto rng = std::make_unique<Mt19937Rng>();
	Dice d(sides, std::move(rng));

	WelfordAcc acc;
	resetAcc(acc, sides);

	std::vector<int> counts(sides + 1, 0);
	const int total = dice * trials;

	for (int b = 0; b < batches; ++b) {
		rollCounts(d, total, counts);
		welfordUpdate(acc.mean, acc.m2, acc.n, counts, sides, total);
	}

	return acc;
}

//run functions
void Simulator::run(const Dice& d) {
	for (int i = 0; i < trials; i++) {
		for (int j = 0; j < dice; j++) {
			++faceCount[d.roll()];
		}
	}
}
void Simulator::runBatches(const Dice& d, int batches) {
	resetFinalStats();                               // mean/m2/n_batches = 0
	std::vector<int> counts(sides + 1, 0);
	const int total = dice * trials;

	for (int b = 0; b < batches; ++b) {
		rollCounts(d, total, counts);
		welfordUpdate(mean, m2, n_batches, counts, sides, total);
	}
}
void Simulator::runBatchesParallel(int total_batches, int num_threads) {
	if (total_batches <= 0) return;
	if (num_threads < 1)    num_threads = 1;

	resetFinalStats();                               // mean/m2/n_batches = 0

	const int base = total_batches / num_threads;
	const int extra = total_batches % num_threads;

	std::vector<WelfordAcc> locals(num_threads);
	std::vector<std::thread> threads;
	threads.reserve(num_threads);

	// launch workers, each fills locals[t]
	for (int t = 0; t < num_threads; ++t) {
		const int my_batches = base + (t < extra ? 1 : 0);
		threads.emplace_back([&, t, my_batches] {
			locals[t] = Worker(my_batches, sides, dice, trials);
			});
	}
	for (auto& th : threads) th.join();

	// compute total batches once
	long long totalMerged = 0;
	for (int t = 0; t < num_threads; ++t) totalMerged += locals[t].n;

	// merge each face independently
	for (int f = 1; f <= sides; ++f) {
		long long N = 0;     // merged count for THIS face
		double mean_f = 0.0;
		double m2_f = 0.0;

		for (int t = 0; t < num_threads; ++t) {
			const auto& acc = locals[t];
			if (acc.n == 0) continue;

			MergeData md = merge(N, mean_f, m2_f, acc.n, acc.mean[f], acc.m2[f]);

			N = md.n;
			mean_f = md.mean;
			m2_f = md.m2;
		}

		mean[f] = mean_f;
		m2[f] = m2_f;
	}

	n_batches = totalMerged;
}

//Misc. functions
void Simulator::InsertionSort() {

	sortedFaces.clear();
	sortedFaces.reserve(sides);
	for (int n = 0; n < sides; n++) {
		sortedFaces.push_back(std::make_pair(n + 1, faceCount[n + 1]));
	}


	for (int i = 1; i < sortedFaces.size(); i++) {

		auto key = sortedFaces[i];
		int j = i - 1;

		for (j; j >= 0; j--) {
			if (sortedFaces[j].second < key.second || (sortedFaces[j].second == key.second
				&& sortedFaces[j].first > key.first)) {

				sortedFaces[j + 1] = sortedFaces[j];
				continue;
			}
			break;
		}
		sortedFaces[j + 1] = key;

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
			<< "," << rel_error << "\n";
	}

	file.close();
	std::cout << "Results exported to " << filename << "\n";
}

//Display functions
void Simulator::displayWelford() const {
	std::cout << "Results for batches: \n";
	for (int i = 1; i < faceCount.size(); i++) {
		double variance = m2[i] / std::max(1LL, n_batches - 1);
		double stddev = std::sqrt(variance);

		double ab_error = std::abs(mean[i] - probability);
		double rel_error = ab_error / probability * 100;
		std::cout << " Face: " << i << " Probability: " << mean[i] << " StdDev: " << stddev << " Abs Error: " << ab_error
			<< " Rel Error: " << rel_error << "%\n";
	}
}
void Simulator::displaySorted() const {
	std::cout << "\nSorted Results (by count):\n";
	for (int n = 0; n < sortedFaces.size(); n++) {
		std::cout << "Face: " << sortedFaces[n].first << " Count: " << sortedFaces[n].second << "\n";
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
			<< " , " << "Relative Error: " << rel_error << "%\n";
	}
}