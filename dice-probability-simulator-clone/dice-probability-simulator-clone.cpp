
// Dice face frequency (classic style)
// - RNG: rand()/srand()
// - Counters: C-style array
// - Output: simple columns

#include <iostream>
#include <cstdlib>
#include <ctime>

int main() {
    std::cout << "Dice:\n";
    int dice = 0;
    if (!(std::cin >> dice) || dice <= 0) {
        std::cerr << "Invalid dice number.\n";
        return 1;
    }

    std::cout << "Trials:\n";
    int trials = 0;
    if (!(std::cin >> trials) || trials <= 0) {
        std::cerr << "Invalid number of trials.\n";
        return 1;
    }

    // seed PRNG once at program start
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    long long faceCount[7] = { 0 }; // use indices 1..6

    // simulate: for each trial, roll 'dice' times
    for (int t = 0; t < trials; ++t) {
        for (int i = 0; i < dice; ++i) {
            int r = std::rand() % 6 + 1; // 1..6
            ++faceCount[r];
        }
    }

    long long totalRolls = static_cast<long long>(dice) * trials;

    std::cout << "\nFace  Count  Probability\n";
    for (int face = 1; face <= 6; ++face) {
        double p = static_cast<double>(faceCount[face]) / totalRolls;
        std::cout << "  " << face << "   " << faceCount[face]
            << "    " << p << "\n";
    }

    return 0;
}
