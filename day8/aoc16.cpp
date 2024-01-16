#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <map>
#include <utility>
#include <bitset>
#include <array>
#define WIDTH 25
#define HEIGHT 6

template <int W, int H>
void printArray(std::array<std::array<int, W>, H> grid) {
    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) {
            std::cout << char((grid[y][x] == 0) ? ' ' : '#');
        } std::cout << std::endl;
    } std::cout << std::endl;
}

int main() {
    std::ifstream input ("input15.txt");
    std::map<uint64_t, uint64_t> scores;
    std::array<std::array<int, WIDTH>, HEIGHT> found;
    for (auto& a : found) {a.fill(2);}
    while(!input.eof()) {
        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; x++) {
                char c = input.get() - '0';
                if (c == 2) {continue;}
                else if (found[y][x] == 2) {
                    found[y][x] = c;
                }
            }
        }
    }
    printArray<WIDTH, HEIGHT>(found);
    return 0;
}