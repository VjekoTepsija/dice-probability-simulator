#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Dice.h"
#include "Simulator.h"

int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    std::cout << "Dice sides, max is 100: \n";
    int sides = 0;
    std::cin >> sides;
    if (sides < 2 || sides > 100) {
        std::cerr << "Invalid number of sides. Setting sides to 6\n";
        sides = 6;
    }

    std::cout << "Number of dice:\n";
    int dice = 0;
    std::cin >> dice;
    if (dice < 1) {
        std::cerr << "Invalid dice number.\n";
        return 1;
    }

    std::cout << "Trials:\n";
    int trials = 0;
    if (!(std::cin >> trials) || trials <= 0) {
        std::cerr << "Invalid number of trials.\n";
        return 1;
    }

    Dice d(sides);
    Simulator sim(dice, trials, sides);

    sim.run(d);
    sim.display();
    sim.exportCSV("results.csv");

    sim.InsertionSort();

    return 0;
}
