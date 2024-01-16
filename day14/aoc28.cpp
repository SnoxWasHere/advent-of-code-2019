#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <set>
#include <map>
#include <utility>
#include <sstream>
#include <unordered_map>

typedef std::string chem_t;
struct Ingredient {
    int64_t quantity;
    chem_t chemical;
};
struct Recipe {
    Ingredient output;
    std::vector<Ingredient> inputs;
};

Ingredient process(std::string line) {
    Ingredient out;
    int pos = line.find(" ");
    out.quantity = std::stoi(line.substr(0, pos));
    out.chemical = line.substr(pos + 1);
    return out;
}

struct Analyzer {
    std::unordered_map<chem_t, Recipe> recipes;
    std::unordered_map<chem_t, int64_t> surplus;
    int64_t count = 0;
    int64_t costOf(const Ingredient ing) {
        if (ing.chemical == "ORE") {return ing.quantity;}
        const Recipe& rec = recipes[ing.chemical];
        int64_t bonus = 0;
        if (surplus.contains(ing.chemical)) {
            bonus = surplus.at(ing.chemical);
        }
        int64_t batchSize = rec.output.quantity;
        int64_t mult = (ing.quantity - bonus + batchSize - 1) / batchSize;
        //surplus == (how much we made) - (how much we need);
        surplus[ing.chemical] = ((batchSize*mult + bonus) - ing.quantity);
        if(mult == 0) {return 0;}
        int64_t cost = 0;
        for (const auto& i : rec.inputs) {
            cost += costOf(Ingredient{i.quantity*mult, i.chemical});
        }
        return cost;
    }
};

int64_t looseSearch(Analyzer& a, int64_t ore, int64_t size) {
    int64_t initial = a.costOf({size, "FUEL"});
    int64_t loops = (ore / initial) - 1;
    ore -= initial;
    a.count += size;
    for (int i = 0; i < loops; i++) {
        ore -= a.costOf({size, "FUEL"});
        a.count += size;;
    }
    return ore;
}

//newline instead of =>
//',' instead of ", "
int main() {
    std::ifstream input("input27.txt", std::ios::in); 
    std::unordered_map<chem_t, Recipe> inverseRecipes;

    while (!input.eof()) {
        Recipe cur;
        std::string line; std::getline(input, line);
        std::stringstream ss(line);
        while (!ss.eof()) {
            std::getline(ss, line, ',');
            cur.inputs.push_back(process(line));
        }
        std::getline(input, line);
        cur.output = process(line);
        inverseRecipes.insert({cur.output.chemical, cur});
    } input.close();
    Analyzer a {inverseRecipes};
    int64_t ore = 1'000'000'000'000;
    for (int x = 1'000'000; x > 1; x /= 10) {
        ore = looseSearch(a, ore, x);
    }
    while (ore > 0) {
        ore -= a.costOf({1, "FUEL"});
        a.count++;
    }
    std::cout << --a.count << std::endl;
    return 0;
}