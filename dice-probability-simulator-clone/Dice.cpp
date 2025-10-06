#include "Dice.h"

Dice::Dice(int s) : sides(s) {}

int Dice::roll() const {
    return 1 + std::rand() % sides;
}