#pragma once
#include <cstdlib>

class Dice {
private:
    int sides;

public:
    Dice(int s = 6);
    int roll() const;
};
