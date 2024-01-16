#ifndef _OPCODE_
#define _OPCODE_
#include "intcode.hpp"

struct OpCode {
    static void add(IntCode* ic, modes_t mds) { //1
        val_t lhs = ic->reel.mread(mds[0]);
        val_t rhs = ic->reel.mread(mds[1]);
        ic->reel.mwrite((lhs+rhs), mds[2]);
    }
    static void multiply(IntCode* ic, modes_t mds) { //2
        val_t lhs = ic->reel.mread(mds[0]);
        val_t rhs = ic->reel.mread(mds[1]);
        ic->reel.mwrite((lhs*rhs), mds[2]);
    }
    static void saveInput(IntCode* ic, modes_t mds) { //3
        std::optional<val_t> inval = ic->iread();
        if (!inval.has_value()) {
            --(ic->reel); //go back to beginning of input instruction
            ic->exit(exit_t::pause);
            return;
        }
        ic->reel.mwrite((inval.value()), mds[0]); 
    }
    static void saveOutput(IntCode* ic, modes_t mds) { //4
        ic->output.push_back(ic->reel.mread(mds[0]));
    }
    static void jumpIfTrue(IntCode* ic, modes_t mds) { //5
        val_t val = ic->reel.mread(mds[0]);
        val_t pos = ic->reel.mread(mds[1]);
        if (val > 0) {
            ic->reel.jump(pos - 1);
        }
    }
    static void jumpIfFalse(IntCode* ic, modes_t mds) { //6
        val_t val = ic->reel.mread(mds[0]);
        val_t pos = ic->reel.mread(mds[1]);
        if (val == 0) {
            ic->reel.jump(pos - 1);
        }
    }
    static void less(IntCode* ic, modes_t mds) { //7
        val_t lhs = ic->reel.mread(mds[0]);
        val_t rhs = ic->reel.mread(mds[1]);
        ic->reel.mwrite((lhs < rhs), mds[2]);
    }
    static void equals(IntCode* ic, modes_t mds) { //8
        val_t lhs = ic->reel.mread(mds[0]);
        val_t rhs = ic->reel.mread(mds[1]);
        ic->reel.mwrite((lhs == rhs), mds[2]);
    }
    static void updateOffset(IntCode* ic, modes_t mds) { //9
        val_t val = ic->reel.mread(mds[0]);
        ic->reel.update_ofs(val);
    }
    static void halt(IntCode* ic, modes_t mds) { //99
        ic->exit(exit_t::halt);
    }

    static oper_t getOperator(val_t val) {
            switch (val % 100) {
        case 1:
            return OpCode::add;
        case 2:
            return OpCode::multiply;
        case 3:
            return OpCode::saveInput;
        case 4:
            return OpCode::saveOutput;
        case 5:
            return OpCode::jumpIfTrue;
        case 6:
            return OpCode::jumpIfFalse;
        case 7:
            return OpCode::less;
        case 8:
            return OpCode::equals;
        case 9:
            return OpCode::updateOffset;
        case 99:
            return OpCode::halt;
        default:
            std::unreachable();
        }
    }
};

#endif