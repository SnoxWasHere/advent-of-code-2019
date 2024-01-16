#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>

int main() {
    std::ifstream input ("input1.txt", std::ios::in);
    uint64_t count = 0;
    while (!input.eof()) {
        std::string line; std::getline(input, line);
        int64_t weight = std::stoll(line);
        while(true) {
            weight = (weight / 3) - 2;
            if (weight <= 0) {break;}
            count += weight;
        } 
    }
    std::cout << count << std::endl;
}