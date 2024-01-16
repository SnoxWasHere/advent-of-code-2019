#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>
#include <cstdint>
#include <set>
#define sqrd(x) (x)*(x)

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

int main() {
    std::ifstream input("input19.txt", std::ios::in);
    std::vector<Coord> ast;
    std::vector<std::vector<char>> grid;
    int16_t y = 0;
    while (!input.eof()) {
        std::string line; std::getline(input, line);
        grid.emplace_back();
        for (int16_t x = 0; x < line.size(); x++) {
            grid.back().push_back(line[x]);
            if (line[x] == '#') {
                ast.push_back(Coord{x, y});
            }
        }
        y++;
    }
    
    int highest = 0;
    for (const auto& start : ast) {
        std::set<Coord> visible;
        for (const auto& end : ast) {
            if (start == end) {continue;}
            bool blocked = false;
            Coord diff = end - start;
            Coord step = diff / std::gcd(diff.x, diff.y);
            Coord loc = start + step;
            while (loc != end) {
                if (grid[loc.y][loc.x] == '#') {blocked = true; break;}
                loc = loc + step;
            }

            if (!blocked) {visible.insert(end);}
        }
        if (visible.size() > highest) {highest = visible.size();}
    }
    std::cout << highest << std::endl;
    return 0;
}