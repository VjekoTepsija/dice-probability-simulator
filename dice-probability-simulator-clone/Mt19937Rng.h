#pragma once
#include "IRng.h"
#include <random>

class Mt19937Rng :public IRng {
private:
	std::mt19937 gen;
public:

	int nextInt(int min, int max) override {
		if (min > max) {
			int temp = min;
			min = max;
			max = temp;
		}
		std::uniform_int_distribution<int> dist(min, max);

		return dist(gen);
	}
	Mt19937Rng() {
		std::random_device rd;
		gen = std::mt19937(rd());
	}
	Mt19937Rng(unsigned seed) {
		gen = std::mt19937(seed);
	}
};