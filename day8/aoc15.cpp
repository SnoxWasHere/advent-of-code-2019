#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <map>
#include <utility>
#define WIDTH 25
#define HEIGHT 6

int main() {
    std::ifstream input ("input15.txt");
    std::map<uint64_t, uint64_t> scores;
    while(true) {
        uint64_t zeros = 0;
        uint64_t ones = 0;
        uint64_t twos = 0;
        for (int x = 0; x < WIDTH; x++) {
            for (int y = 0; y < HEIGHT; y++) {
                char c = input.get();
                switch (c) {
                case '0':
                    zeros++; break;
                case '1':
                    ones++; break;
                case '2':
                    twos++; break;
                default:
                    std::unreachable();
                }
            }
        }
        if (!input.eof()) {scores.insert({zeros, ones*twos});}
        else {break;}
    }
    std::cout << (*scores.begin()).second << std::endl;
    return 0;
}