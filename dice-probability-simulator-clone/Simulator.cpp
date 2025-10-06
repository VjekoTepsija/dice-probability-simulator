#include "Simulator.h"
#include <fstream>
#include <iostream>
#include <iomanip>

Simulator::Simulator(int dice, int trials, int sides)
    : dice(dice), trials(trials), sides(sides) {
    faceCount.resize(sides + 1, 0);
}

long long Simulator::totalRolls() const {
    return static_cast<long long>(dice) * trials;
}

void Simulator::run(const Dice& d) {
    for (int i = 0; i < trials; i++) {
        for (int j = 0; j < dice; j++) {
            ++faceCount[d.roll()];
        }
    }
}

void Simulator::display() const {
    std::cout << "\nFace  Count  Probability\n";
    for (int face = 1; face <= sides; face++) {
        double p = static_cast<double>(faceCount[face]) / totalRolls();
        std::cout << face << "     " << faceCount[face] << "      " << p << "\n";
    }
}

void Simulator::exportCSV(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Failed to open results file.\n";
        return;
    }

    file << "Face,Count,Probability\n";
    for (int face = 1; face <= sides; face++) {
        double p = static_cast<double>(faceCount[face]) / totalRolls();
        file << face << "," << faceCount[face] << "," << p << "\n";
    }

    file.close();
    std::cout << "Results exported to " << filename << "\n";
}