#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <set>
#include <map>
#include "../src/intcode.hpp"

enum class tile : uint8_t {
    undiscovered = 99,
    empty = 0,
    wall = 1,
    goal = 2
};

enum class dir : uint8_t {
    north = 1,
    south = 2,
    west = 3,
    east = 4
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

struct Grid {
    std::map<Coord, tile> gd {{Coord{0,0}, tile::empty}};
    tile at(const Coord& i) const {
        if (gd.contains(i)) {return gd.at(i);}
        return tile::undiscovered;
    }
    tile& operator[](const Coord& i) {
        if (!gd.contains(i)) {gd.insert({i, tile::undiscovered});}
        return gd[i];
    }
};

std::map<dir, Coord> cardinals {
    {dir::north, {0,-1}},
    {dir::east,  {1, 0}},
    {dir::south, {0, 1}},
    {dir::west,  {-1,0}},
};

typedef std::vector<dir> path_t;

struct Traveler {
    Coord pos {0, 0};
    Grid gd {};
    IntCode ic;
    Traveler(const std::vector<val_t>& i) : ic(i) {}
    //find first undiscovered square
    path_t floodFill() {
        std::vector<Coord> nodes {pos};
        std::map<Coord, dir> seen {}; //dir is the dir to the seen square
        while (true) {
            std::vector<Coord> newNodes = {};
            for (auto& node : nodes) {
                for (const auto& [d, cardinal] : cardinals) {
                    Coord c = node + cardinal;
                    if (!seen.contains(c)) {
                        seen.insert({c, d});
                        if (gd.at(c) == tile::undiscovered) {
                            //get path back to start
                            Coord cur = c;
                            dir d;
                            path_t path {};
                            do {
                                d = seen[cur];
                                path.push_back(d); 
                                cur = cur - cardinals[d];
                            } while (cur != pos);
                            return path;
                        }
                        if (gd.at(c) != tile::wall) {newNodes.push_back(c);}
                    }
                }
            } nodes = newNodes;
        }
    }
    tile step(dir d) {
        ic.run(val_t(d));
        tile result = tile(ic.getOut());
        Coord cur = pos + cardinals[d];
        gd[cur] = result; 
        if (result != tile::wall) {pos = cur;}
        return result;
    }
    tile investigate(path_t path) {
        for (int x = 1; x < path.size(); x++) {
            step(path[x]);
        }
        return step(path[0]);
    }
    int run() {
        while (true) {
            if (investigate(floodFill()) == tile::goal) {
                //TODO find actual distance
                return (abs(pos.x) + abs(pos.y));
            }
        }
    }
};


int main() {
    std::ifstream input ("input29.txt");
    std::vector<val_t> reel;
    while(!input.eof()) {
        std::string line; std::getline(input, line, ',');
        reel.push_back(std::stoll(line));
    } input.close();

    Traveler t(reel);
    std::cout << t.run() << std::endl;
    return 0;
}