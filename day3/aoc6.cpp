#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdint>
#include <utility>

struct Coord {
    int x, y;
};
Coord operator+(const Coord& lhs, const Coord& rhs) {
    return {lhs.x + rhs.x, lhs.y + rhs.y};
}
Coord operator-(const Coord& lhs, const Coord& rhs) {
    return {lhs.x - rhs.x, lhs.y - rhs.y};
}
bool operator==(const Coord& lhs, const Coord& rhs) {
    return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}

std::vector<Coord> getPositions(std::stringstream& ss) {
    std::vector<Coord> out {{0,0}};
    while (!ss.eof()) {
        std::string instr; std::getline(ss, instr, ',');
        Coord ofs {0,0};
        switch (instr[0]) {
        case 'U':
            ofs = {0,-1}; break;
        case 'R':
            ofs = {1, 0}; break;
        case 'D':
            ofs = {0, 1}; break;
        case 'L':
            ofs = {-1,0}; break;
        default:
            std::unreachable();
        }
        int steps = std::stoi(instr.substr(1));
        Coord pos = out.back();
        for (int i = 0; i < steps; i++) {
            pos = pos + ofs;
            out.push_back(pos);
        }
    }
    return out;
}

int main() {
    std::ifstream input("input5.txt", std::ios::in);
    std::vector<Coord> lhs {{0,0}};
    std::vector<Coord> rhs {{0,0}};
    while (!input.eof()) {
        std::string line; std::getline(input, line);
        std::stringstream lsline;
        lsline << line;
        lhs = getPositions(lsline);

        std::getline(input, line);
        std::stringstream rsline;
        rsline << line;
        rhs = getPositions(rsline);
    }

    int manhattan = INT32_MAX;
    for (int l = 0; l < lhs.size(); l++) {
        for (int r = 0; r < rhs.size(); r++) {
            if (lhs[l] == rhs[r]) {
                int dist = l + r;
                if ((dist != 0) && (dist < manhattan)) {manhattan = dist;}
            }
        }
    }
    std::cout << manhattan << std::endl;
    return 0;
}