#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <set>
#include <map>
#include "../src/intcode.hpp"

enum class dir : int8_t {
    north = 0,
    east = 1,
    south = 2,
    west = 3
};

struct Coord {
    int x, y;
    auto operator<=>(const Coord& other) const = default;
};
Coord operator+(const Coord& lhs, const Coord& rhs) {
    return {lhs.x + rhs.x, lhs.y + rhs.y};
}
Coord operator-(const Coord& lhs, const Coord& rhs) {
    return {lhs.x - rhs.x, lhs.y - rhs.y};
}

std::map<dir, Coord> cardinals {
    {dir::north, Coord{0, -1}},
    {dir::east, Coord{1, 0}},  
    {dir::south, Coord{0, 1}},  
    {dir::west, Coord{-1, 0}}
};

int main() {
    std::ifstream input ("input21.txt");
    std::vector<val_t> reel;
    while(!input.eof()) {
        std::string line; std::getline(input, line, ',');
        reel.push_back(std::stoll(line));
    } input.close();

    IntCode ic(reel);

    std::set<Coord> whiteSquares {};
    std::set<Coord> squares {};
    Coord pos {0, 0};
    dir current = dir::north;
    exit_t status = exit_t::running;
    while (status != exit_t::halt) {
        bool isWhite = whiteSquares.contains(pos);
        status = ic.run(int(isWhite));
        std::pair<int,int> instr = {ic.getOut(0), ic.getOut(1)};
        squares.insert(pos);
        if (instr.first == 1) {
            whiteSquares.insert(pos);
        } else if (whiteSquares.contains(pos)) {
            whiteSquares.erase(pos);
        }
        current = dir((int8_t(current) + instr.second*2 + 3) % 4);
        pos = pos + cardinals[current];
        ic.clearOut();
    }
    std::cout << squares.size() << std::endl;
    return 0;
}