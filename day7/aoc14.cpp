#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include "../src/intcode.hpp"

val_t testConfig(std::vector<val_t>& reel, int a, int b, int c, int d, int e) {
    std::vector<IntCode> ics {IntCode(reel, a), IntCode(reel, b), IntCode(reel, c), IntCode(reel, d), IntCode(reel, e)};
    val_t in = 0;
    while (true) {
        exit_t status = exit_t::running;
        for (auto& ic : ics) {
            ic.addInput(in);
            status = ic.run();
            in = ic.getOut();   
        }
        if (status == exit_t::halt) {return in;}
    }
}


int main() {
    std::ifstream input ("input13.txt");
    std::vector<val_t> reel;
    while(!input.eof()) {
        std::string line; std::getline(input, line, ',');
        reel.push_back(std::stoll(line));
    }
    uint64_t highest = 0;
    uint64_t setting = 0;
    const int MIN = 5, MAX = 9;
    for (int a = MIN; a <= MAX; a++) {
        for (int b = MIN; b <= MAX; b++) {
            if (b == a) {continue;}
            for (int c = MIN; c <= MAX; c++) {
                if (c == a || c == b) {continue;}
                for (int d = MIN; d <= MAX; d++) {
                    if (d == a || d == b || d == c) {continue;}
                    int e = 35 - (a+b+c+d);
                    uint64_t in = testConfig(reel,a,b,c,d,e);
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