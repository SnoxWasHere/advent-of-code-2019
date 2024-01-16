#ifndef _INTCODE_
#define _INTCODE_

#include <functional>
#include <utility>
#include <optional>
#include "modes.hpp"
#include "reel.hpp"

class IntCode;
typedef std::function<void(IntCode*, modes_t)> oper_t;

enum class exit_t : uint8_t {
    running,
    halt,
    pause
};

class IntCode {
private:
    exit_t status = exit_t::running;
    void exit(exit_t t) {
        status = t;
    }

    reel_t reel;

    std::vector<val_t> input {};
    pos_t indx = 0;
    //Handles reading from the input buffer
    //Returns nullopt if input doesn't exist
    std::optional<val_t> iread() {
        if (indx >= input.size()) {return std::nullopt;} 
        val_t out = input[indx];
        indx++;
        return out;
    }
    
    std::vector<val_t> output {};
    
public:
    friend struct OpCode;
    IntCode(std::vector<val_t> r) : reel(r) {}
    IntCode(std::vector<val_t> r, val_t i) : reel(r), input({i}) {}
    IntCode(std::vector<val_t> r, std::vector<val_t> i) : reel(r), input(i) {}

    //Runs until hits a 99
    //Returns exit type
    exit_t run();

    //runs with new input
    exit_t run(val_t x);

    //Appends input to vector
    void addInput(val_t x) {
        input.push_back(x);
    }

    //Prints out entire output vec
    void printOutput() {
        for (const auto& o : this->output) {
            std::cout << o << ',';
        } std::cout << std::endl;
    }
    std::vector<val_t> outputReel() {
        return output;
    }

    //Gets last part of output
    val_t getOut() {
        return output.back();
    }
    val_t getOut(pos_t p) {
        if (p >= output.size()) {std::unreachable();}
        return output[p];
    }

    void clearOut() {
        output.clear();
    }
};

/*********************** BEGIN BODY ***********************/
#include "opcode.hpp"

exit_t IntCode::run() {
    this->status = exit_t::running;
    while (this->status == exit_t::running) {
        int64_t instr = reel.mread(mode::immediate);
        oper_t oper = OpCode::getOperator(instr);
        oper(this, modes_t::getMode(instr));
    }
    return this->status;
}

exit_t IntCode::run(val_t x) {
    this->addInput(x);
    return this->run();
}


#endif