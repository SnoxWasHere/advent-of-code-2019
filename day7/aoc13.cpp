#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include "../src/intcode.hpp"

int main() {
    std::ifstream input ("input13.txt");
    std::vector<val_t> reel;
    while(!input.eof()) {
        std::string line; std::getline(input, line, ',');
        reel.push_back(std::stoll(line));
    }
    uint64_t highest = 0;
    uint64_t setting = 0;
    for (int a = 0; a <= 4; a++) {
        for (int b = 0; b <= 4; b++) {
            if (b == a) {continue;}
            for (int c = 0; c <= 4; c++) {
                if (c == a || c == b) {continue;}
                for (int d = 0; d <= 4; d++) {
                    if (d == a || d == b || d == c) {continue;}
                    int e = 10 - (a+b+c+d);
                    int in = 0;
                    for (const auto& phase : {a,b,c,d,e}) {
                        IntCode ic(reel, {phase, in});
                        ic.run();
                        in = ic.getOut();
                    }
                    if (in > highest) {
                        highest = in;
                        setting = e + 10*d + 100*c + 1000*b + 10000*a;
                    }
                }
            }
        }
    }
    std::cout << highest << " - " << setting << std::endl;
}