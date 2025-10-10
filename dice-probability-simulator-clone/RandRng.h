#pragma once
#include "IRng.h"
#include <cstdlib>
#include <ctime>

class RandRng :public IRng {
public:
	RandRng(unsigned seed = static_cast<unsigned>(std::time(nullptr))) {
		std::srand(seed);
	}
	int nextInt(int min, int max) override{
		if (min > max) {
			int temp = min;
			min = max;
			max = temp;
		}
		if (min == max)
			return min;
		int span = max - min + 1;
		return std::rand() % span + min;

	}
};