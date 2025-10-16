#include "Dice.h"
#include <memory>

 Dice::Dice(int s, std::unique_ptr<IRng> r) : sides(s),rng(std::move(r)){
	if (this->sides < 2) {
		this->sides = 6;
	}
}

int Dice::roll() const{
	return rng->nextInt(1, sides);
}