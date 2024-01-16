#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include "../src/intcode.hpp"

int main() {
    std::ifstream input ("input3.txt");
    std::vector<val_t> reel;
    while(!input.eof()) {
        std::string line; std::getline(input, line, ',');
        reel.push_back(std::stoll(line));
    }
    for (int x = 0; x <= 99; x++) {
        for (int y = 0; y <= 99; y++) {
            auto newreel = reel;
            reel[1] = x;
            reel[2] = y;
            IntCode ic(reel);
            ic.run();
            // if (ic.read(0) == 19690720) {
            //     std::cout << ((100 * x) + y) << std::endl;
            // }
        }
    }
}