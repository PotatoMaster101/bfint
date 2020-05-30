////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Simple Brainfuck interpretor written in C++17.
//
// Author: PotatoMaster101
// Date:   30/05/2020
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdexcept>
#include "bfint.hpp"

// Indicates the start of a loop.
//
// EXCEPT:
// std::runtime_error - when end of loop is not found
bfint::size_type bfint::start_loop() {
    if (cell() == 0)    // don't run if loop counter is 0
        return find_loop_end() + 1;

    loopstk_.push(instptr_);
    return instptr_ + 1;
}

// Indicates the end of a loop.
//
// EXCEPT:
// std::runtime_error - when start of loop is not found
bfint::size_type bfint::end_loop() {
    if (!inloop())
        throw std::runtime_error("loop did not start");

    if (cell() == 0) {      // loop counter is 0, quit
        loopstk_.pop();
        return instptr_ + 1;
    }
    return loopstk_.top() + 1;
}

// Runs the Brainfuck code.
//
// EXCEPT:
// std::runtime_error - when shit happens
void bfint::run() {
    while (instptr_ < insts_.size()) {
        auto iter = TOKEN_MAP.find(insts_[instptr_]);
        if (iter == TOKEN_MAP.end()) {
            ++instptr_;
            continue;       // not a Brainfuck instruction, skip
        }
        switch (iter->second) {     // run current instruction
            case bftoken::inc: inc(); ++instptr_; break;
            case bftoken::dec: dec(); ++instptr_; break;
            case bftoken::lsh: lshift(); ++instptr_; break;
            case bftoken::rsh: rshift(); ++instptr_; break;
            case bftoken::out: output_ch(); ++instptr_; break;
            case bftoken::in: input_ch(); ++instptr_; break;
            case bftoken::lstart: instptr_ = start_loop(); break;
            case bftoken::lend: instptr_ = end_loop(); break;
        }
    }
    if (inloop())
        throw std::runtime_error("end of loop not found");
}

// Returns the instruction index of the end of current loop (the ] token).
//
// EXCEPT:
// std::runtime_error - when end of loop is not found
bfint::size_type bfint::find_loop_end() const {
    auto opencnt = 1;
    for (auto i = instptr_ + 1; i < insts_.size(); i++) {
        if (insts_[i] == '[')
            opencnt++;
        else if (insts_[i] == ']')
            opencnt--;

        if (opencnt == 0)
            return i;
    }
    throw std::runtime_error("end of loop not found");
}
