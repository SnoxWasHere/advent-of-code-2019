#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <set>
#include <map>
#include <conio.h>
#include "../src/intcode.hpp"

enum class tile : uint8_t {
    empty = 0,
    wall = 1,
    block = 2,
    paddle = 3,
    ball = 4
};

struct Coord {
    int64_t x, y;
    auto operator<=>(const Coord& other) const = default;
};

std::pair<Coord, Coord> getBounds(const std::map<Coord, tile>& screen) {
    //top left, bottom right
    std::pair<Coord, Coord> bounds {{INT32_MAX, INT32_MAX}, {INT32_MIN, INT32_MIN}};
    for (const auto& [s, val] : screen) {
        if (s == Coord{-1, 0}) {continue;}
        if (s.x < bounds.first.x) {bounds.first.x = s.x;}
        if (s.y < bounds.first.y) {bounds.first.y = s.y;}

        if (s.x > bounds.second.x) {bounds.second.x = s.x;}
        if (s.y > bounds.second.y) {bounds.second.y = s.y;}
    }
    return bounds;
}

std::unordered_map<tile, char> toChar {
    {tile::empty,  ' '},
    {tile::wall,   '+'},
    {tile::block,  '#'},
    {tile::paddle, '-'},
    {tile::ball,   'O'}
};

void printScreen(const std::map<Coord, tile>& screen, const std::pair<Coord, Coord>& bounds) {
    for (int y = bounds.first.y; y <= bounds.second.y; y++) {
        for (int x = bounds.first.x; x <= bounds.second.x; x++) {
            if (screen.contains({x, y})) {
                std::cout << toChar[screen.at({x, y})];
            } else {
                std::cout << toChar[tile::empty];
            }
        } std::cout << '\n';
    } std::cout << '\n';
}

int autoplay(const std::map<Coord, tile>& screen) {
    Coord paddle;
    Coord ball;
    for (const auto [c, t] : screen) {
        if (t == tile::paddle) {
            paddle = c;
        } else if (t == tile::ball) {
            ball = c;
        }
    }
    if (paddle.x < ball.x) {return 1;}
    if (paddle.x > ball.x) {return -1;}
    else {return 0;}
}

void play(std::vector<val_t> &reel, const int vwmode = 1, const int inmode = 1) {
    IntCode ic(reel);
    exit_t ex = ic.run();
    std::optional<std::pair<Coord, Coord>> bounds = std::nullopt;
    std::map<Coord, tile> screen;
    int score = 0;
    while (true) {
        auto out = ic.outputReel();
        for (int i = 0; i < out.size(); i += 3) {
            Coord instr {out[i], out[i+1]};
            int val = out[i+2];
            if (instr == Coord{-1, 0}) {
                score = val;
            } else {
                screen.insert_or_assign(instr, tile(val));
            }
        } 
        int instr = 0;
        if (vwmode == 1) {
            std::cout << score << std::endl;
            if (!bounds.has_value()) {bounds = getBounds(screen);}
            printScreen(screen, bounds.value());
            char in = _getch();
            if (inmode == 1) {
                switch (in) {
                case 'a':
                    instr = -1;
                    break;
                case 'd':
                    instr = 1;
                    break;
                default:
                    break;
                }
            }
        }
        if (inmode == 2) {
            instr = autoplay(screen);
        }
        ic.clearOut();
        if (ex == exit_t::halt) {break;}
        ex = ic.run(instr);
    }
    if (vwmode == 2) {std::cout << score << std::endl;}
}

int main() {
    std::ifstream input ("input26.txt");
    std::vector<val_t> reel;
    while(!input.eof()) {
        std::string line; std::getline(input, line, ',');
        reel.push_back(std::stoll(line));
    } input.close();
    
    /****** MODE SELECT *****/
    // 1 - PLAYER
    // 2 - AUTO
    play(reel, 2, 2); 
    return 0;
}