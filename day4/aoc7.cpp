#include <iostream>
#include <cstdint>
#include <sstream>

#define LOWER 158126
#define UPPER 624574

bool checkPassword(uint64_t pwd) {
    std::stringstream ss;
    ss << int(pwd);
    std::string spwd = ss.str();
    int streak = 0;
    bool concurrent = false;
    char previous = '0';
    for (int x = 0; x < 6; x++) {
        char cur = spwd[x];
        if (cur < previous) {return false;}
        if (cur == previous) {streak++;}
        else {
            if (streak == 2) {concurrent = true;}
            streak = 1;
        }
        previous = cur;
    }
    if (streak == 2) {concurrent = true;}
    return concurrent;
};

int main() {
    uint64_t count = 0;
    for (uint64_t x = LOWER; x <= UPPER; x++) {
        if (checkPassword(x)) {count++;}
    }
    std::cout << count << std::endl;
    return 0;
}