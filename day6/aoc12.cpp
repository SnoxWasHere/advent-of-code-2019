#include <vector>
#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <unordered_set>
struct Planet {
    std::string name;
    std::vector<Planet*> neighbors;
};

int bfs(Planet* start, Planet* end) {
    std::unordered_set<std::string> seen;
    std::vector<Planet*> search {start};
    seen.insert(start->name);
    int count = -1; //starting at start's orbit
    while (true) {
        std::vector<Planet*> newsearch;
        for (auto cur : search) {
            for (auto chl : cur->neighbors) {
                if (chl == end) {
                    return count; //end when in same orbit as end
                }
                if (!seen.contains(chl->name)) {
                    newsearch.push_back(chl);
                    seen.insert(chl->name);
                } 
            }
        }
        count++;
        search = newsearch;
    }
}

int main() {
    std::ifstream input("input11.txt", std::ios::in);
    std::map<std::string, std::set<std::string>> strmap;
    while (!input.eof()) {
        std::string line; std::getline(input, line);
        int pos = line.find(")");
        std::string lhs = line.substr(0, pos), rhs = line.substr(pos+1);
        if (!strmap.contains(lhs)) {
            strmap.insert({lhs, {}});
        }
        if (!strmap.contains(rhs)) {
            strmap.insert({rhs, {}});
        }
        strmap[lhs].insert(rhs);
        strmap[rhs].insert(lhs);
    }
    std::map<std::string, Planet*> convert;
    for (const auto& [n, set] : strmap) {
        Planet* p = new Planet{n};
        convert.insert({n, p});
    }
    for (const auto& [n, set] : strmap) {
        Planet*& cur = convert[n];
        for (const auto& chl : set) {
            cur->neighbors.push_back(convert[chl]);
        }
    }

    std::cout << bfs(convert["YOU"], convert["SAN"]) << std::endl;
    for (auto& [n, p] : convert) {delete p;}
    return 0;
}