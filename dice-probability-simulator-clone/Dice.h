#pragma once
#include "IRng.h"
#include <memory>

class Dice {
private:
	int sides;
	std::unique_ptr<IRng> rng;

public:
	explicit Dice(int s, std::unique_ptr<IRng> r);
	int roll() const;
};
