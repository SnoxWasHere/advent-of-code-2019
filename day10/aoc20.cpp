#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>
#include <cstdint>
#include <set>
#include <map>
#include <cmath>
#include <numbers>
#define sqrd(x) (x)*(x)
#define STX 30
#define STY 34
#define SIZE 43


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
template <typename T>
Coord operator/(const Coord& lhs, const T& d) {
    return {lhs.x/d, lhs.y/d};
}

bool outOfBounds(Coord c) {
    return (c.x < 0 || c.x >= SIZE || c.y < 0 || c.y >= SIZE);
}

//30, 34

int main() {
    std::ifstream input("input19.txt", std::ios::in);
    std::vector<std::vector<char>> grid;
    while (!input.eof()) {
        std::string line; std::getline(input, line);
        grid.emplace_back();
        for (int16_t x = 0; x < line.size(); x++) {
            grid.back().push_back(line[x]);
        }
    }
    
    Coord starting {STX, STY};
    std::map<double, Coord, std::greater<double>> angles;
    for (int y = 0; y < grid.size(); y++) {
        for (int x = 0; x < grid[y].size(); x++) {
            Coord cur = Coord{x,y} - starting;
            if (cur == Coord{0,0}) {continue;}
            cur = cur / std::gcd(cur.x, cur.y);
            double angle = atan2(-cur.y, cur.x);
            angle -= std::numbers::pi/2.0;
            angles.insert({angle, cur});
        }
    }
    std::map<double, Coord>::iterator itr = angles.find(0.0);
    int count = 0;
    Coord last = {0,0};
    while (count < 200) {   
        if(itr == angles.end()) {
            itr = angles.begin();
        }
        Coord step = (*itr).second;
        Coord loc = starting + step;
        while (!outOfBounds(loc)) {
            char& square = grid[loc.y][loc.x];
            if (square == '#') {square = '.'; count++; last = loc; break;}
            loc = loc + step;
        }
        itr++;
    }

    std::cout << (last.x*100 + last.y) << std::endl;
    return 0;
}