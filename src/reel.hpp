#ifndef _REEL_
#define _REEL_

#include <utility>
#include <cassert>
#include "modes.hpp"

class reel_t {
private:
    std::vector<val_t> reel;
    pos_t itr = -1;
    pos_t rel = 0;
    
    val_t& access(pos_t pos) {
        if (pos < 0) {std::unreachable();}
        while (pos >= reel.size()) {
            reel.push_back(0);
        }
        return reel[pos];
    } 

    val_t read(pos_t pos) {
        return this->access(pos);
    };
    val_t read(bool advance = true) {
        if (advance) {itr++;}
        return this->access(itr);
    };
    
    void write(val_t val, pos_t pos) {
        this->access(pos) = val;
    };
    void write(val_t val) {
        this->access(itr) = val;
    };
public:
    reel_t(auto r) : reel(r) {}
    //reads with respect to mode
    val_t mread(mode m);
    
    //writes with respect to mode
    void mwrite(val_t val, mode m);

    void jump(pos_t p) {itr = p;}

    //raw read access
    val_t operator[](size_t i) {
        return access(i);
    }
    reel_t& operator++() {itr++; return *this;}
    reel_t& operator--() {itr--; return *this;}

    void update_ofs(pos_t p) {rel += p;}
};

val_t reel_t::mread(mode m) {
    switch (m) {
    case mode::position:
        return this->read(this->read());
    case mode::immediate:
        return this->read();
    case mode::relative:
        return this->read(this->read() + rel);
    default:
        std::unreachable();
    }
}


void reel_t::mwrite(val_t val, mode m) {
    switch (m) {
    case mode::position:
        write(val, this->read()); break;
    case mode::relative:
        write(val, (this->read() + rel)); break;
    default:
        std::unreachable();
    }
}


#endif