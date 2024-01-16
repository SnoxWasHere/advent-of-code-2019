#include <vector>
#include <iostream>
#include <fstream>
#include <map>

struct Planet {
    std::string name;
    Planet* parent;
};
template <typename T>
struct Orbit {
    T lhs;
    T rhs;
};

Orbit<std::string> readOrbit(std::string line) {
    int pos = line.find(")");
    return {line.substr(0, pos), line.substr(pos+1)};
}

int main() {
    std::ifstream input("input11.txt", std::ios::in);
    std::map<std::string, Planet*> orblookup;
    while (!input.eof()) {
        std::string line; std::getline(input, line);
        auto orb = readOrbit(line);
        if (!orblookup.contains(orb.lhs)) {
            Planet* n = new Planet{orb.lhs};
            orblookup.insert({orb.lhs, n});
        } 
        if (!orblookup.contains(orb.rhs)) {
            Planet* n = new Planet{orb.rhs};
            orblookup.insert({orb.rhs, n});
        } 
        if (auto& k = orblookup[orb.rhs]->parent; k == nullptr) {
            k = orblookup[orb.lhs];
        }
    }
    int count = 0;
    for (const auto [n, start] : orblookup) {
        Planet* cur = start;
        while (cur->parent != nullptr) {
            cur = cur->parent;
            count++;
        }
    }
    for (const auto& [n, start] : orblookup) {
        delete start;
    }
    std::cout << count << std::endl;
    return 0;
}