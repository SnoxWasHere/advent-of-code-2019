#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include "../src/intcode.hpp"

int main() {
    std::ifstream input ("input9.txt");
    std::vector<val_t> reel;
    while(!input.eof()) {
        std::string line; std::getline(input, line, ',');
        reel.push_back(std::stoll(line));
    }
    IntCode ic(reel, 1);
    ic.run();
    ic.printOutput();
}