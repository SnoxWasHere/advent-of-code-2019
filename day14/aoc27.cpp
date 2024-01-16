#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <set>
#include <map>
#include <utility>
#include <sstream>

typedef std::string chem_t;
struct Ingredient {
    int quantity;
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
    std::map<chem_t, Recipe> recipes;
    std::map<chem_t, int> surplus;
    int costOf(const Ingredient ing) {
        if (ing.quantity == 0) {return 0;}
        if (ing.chemical == "ORE") {return ing.quantity;}
        const Recipe& rec = recipes[ing.chemical];
        int bonus = 0;
        if (surplus.contains(ing.chemical)) {
            bonus = surplus.at(ing.chemical);
        }
        int batchSize = rec.output.quantity;
        int mult = (ing.quantity - bonus + batchSize - 1) / batchSize;
        //surplus == (how much we made) - (how much we need);
        surplus[ing.chemical] = ((batchSize*mult + bonus) - ing.quantity);

        int cost = 0;
        for (const auto& i : rec.inputs) {
            cost += costOf(Ingredient{i.quantity*mult, i.chemical});
        }
        return cost;
    }
};

//newline instead of =>
//',' instead of ", "
int main() {
    std::ifstream input("input27.txt", std::ios::in); 
    std::map<chem_t, Recipe> inverseRecipes;

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
    std::cout << a.costOf({1, "FUEL"}) << std::endl;
    return 0;
}