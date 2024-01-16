#ifndef _MODES_
#define _MODES_

#include <vector>
#include <cstdint>
typedef int64_t val_t;
typedef int64_t pos_t;

enum class mode : uint8_t {
    position = 0,
    immediate = 1,
    relative = 2
};

struct modes_t {
private:
    std::vector<mode> modes;
public:
    void push_back(mode m) {modes.push_back(m);}
    mode operator[](size_t i) const {
        if (i >= modes.size()) {return mode::position;}
        return modes[i];
    }
    static modes_t getMode(val_t val) {
        modes_t out {};
        val_t mval = val / 100;
        while (mval != 0) {
            out.push_back(static_cast<mode>(mval % 10));
            mval /= 10;
        }
        return out;
    }
};

#endif