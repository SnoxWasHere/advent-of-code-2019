#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <set>
#include <map>
#include "../src/intcode.hpp"

enum class tile : uint8_t {
    empty = 0,
    wall = 1,
    block = 2,
    horizontal = 3,
    ball = 4
};

struct Coord {
    int64_t x, y;
    auto operator<=>(const Coord& other) const = default;
};

int main() {
    std::ifstream input ("input25.txt");
    std::vector<val_t> reel;
    while(!input.eof()) {
        std::string line; std::getline(input, line, ',');
        reel.push_back(std::stoll(line));
    } input.close();
    
    IntCode ic(reel);
    ic.run();

    auto out = ic.outputReel();
    std::map<Coord, tile> screen;
    for (int i = 0; i < out.size(); i += 3) {
        screen.insert({Coord{out[i], out[i+1]}, tile(out[i+2])});
    }

    int count = 0;
    for (const auto& [pix, val] : screen) {
        if (val == tile::block) {count++;}
    }
    std::cout << count << std::endl;
    return 0;
}