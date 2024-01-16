#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <set>
#include <map>
#include <utility>

struct Coord {
    int x, y, z;
    auto operator<=>(const Coord& other) const = default;
    int& operator[](size_t i) {
        switch (i) {
        case 0:
            return x; 
        case 1:
            return y; 
        case 2:
            return z; 
        default:
            std::unreachable();
        }
    }
    int operator[](size_t i) const {
        switch (i) {
        case 0:
            return x; 
        case 1:
            return y; 
        case 2:
            return z; 
        default:
            std::unreachable();
        }
    }
};
Coord operator+(const Coord& lhs, const Coord& rhs) {
    return {lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z};
}
Coord operator-(const Coord& lhs, const Coord& rhs) {
    return {lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z};
}

struct Planet {
    Coord pos {0,0,0};
    Coord vel {0,0,0};
    int idx = -1;
    uint64_t energy() const {
        uint64_t pot = abs(pos.x) + abs(pos.y) + abs(pos.z);
        uint64_t kin = abs(vel.x) + abs(vel.y) + abs(vel.z);
        return pot * kin;
    }
    void pull(Planet& other) {
        for (int i = 0; i <= 2; i++) {
            if (idx >= 0) {i = idx;}
            if (this->pos[i] > other.pos[i]) {
                this->vel[i]--;
                other.vel[i]++;
            }
            else if (this->pos[i] < other.pos[i]) {
                this->vel[i]++;
                other.vel[i]--;
            }
            if (idx >= 0) {break;}
        }
    }
    void step() {
        if (idx >= 0) {this->pos[idx] += this->vel[idx]; return;}
        for (int i = 0; i <= 2; i++) {
            this->pos[i] += this->vel[i];
        }
    }
    bool operator==(const Planet& other) const {
        if (idx >= 0) {
            return (this->pos[idx] == other.pos[idx]) && (this->vel[idx] == other.vel[idx]);
        } else {
            return (this->pos == other.pos) && (this->vel == other.vel);
        }
    }
};

Planet process(std::string line, const std::string delimiter, const int size = 1) {
    std::vector<int> out;
    while (true) {
        int pos = (line.find(delimiter) != std::string::npos) ? line.find(delimiter) : line.length();
        out.push_back(std::stoi(line.substr(0, pos)));
        if (line.length() > pos + size) {
            line.erase(0, pos + size);
        } else {break;}
    } return {{out[0], out[1], out[2]}, {0,0,0}};
}


int main() {
    std::ifstream input ("input23.txt");
    std::vector<Planet> system;
    while(!input.eof()) {
        std::string line; std::getline(input, line);
        system.push_back(process(line, " "));
    }
    
    for (int idx = 0; idx <= 2; idx++) {   
        std::vector<Planet> cur = system; 
        for (auto& c : cur) {c.idx = idx;}
        int count = 0;
        while (++count) {
            for (int a = 0; a < cur.size(); a++) {
                for (int b = a + 1; b < cur.size(); b++) {
                    cur[a].pull(cur[b]);
                }
            }
            for (auto& a : cur) {a.step();}
            if (cur == system) {std::cout << count << std::endl; break;}
        }
    }
    return 0;
}
