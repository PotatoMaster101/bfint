// https://github.com/PotatoMaster101/bfint

#ifndef BFINT_HPP
#define BFINT_HPP
#include <iostream>
#include <ostream>
#include <stack>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

constexpr auto DecrementChar = '-';
constexpr auto IncrementChar = '+';
constexpr auto InputCharChar = ',';
constexpr auto LeftShiftChar = '<';
constexpr auto LoopEndChar = ']';
constexpr auto LoopStartChar = '[';
constexpr auto OutputChar = '.';
constexpr auto RightShiftChar = '>';

/// Brainfuck interpreter.
class bfint {
public:
    /// Constructs a new brainfuck interpreter.
    /// @param instructions String containing the instructions to run.
    /// @param cellSize Starting cell count.
    explicit bfint(std::string instructions, std::size_t cellSize = 30000)
        : instructions_{std::move(instructions)}, cells_(cellSize, 0) {}

    /// Gets the content of the current cell.
    [[nodiscard]] char current_cell() const noexcept {
        return cells_[cell_idx_];
    }

    /// Returns the current instruction position.
    [[nodiscard]] std::size_t current_position() const noexcept {
        return instruction_idx_ + 1;
    }

    /// Decrements the current cell.
    void decrement() noexcept {
        cells_[cell_idx_]--;
    }

    /// Increments the current cell.
    void increment() noexcept {
        cells_[cell_idx_]++;
    }

    /// Reads input from an input stream into the current cell.
    /// @param is The input stream to input from.
    void input(std::istream& is) {
        is >> cells_[cell_idx_];
    }

    /// Move to the cell at left.
    void left_shift() noexcept {
        if (cell_idx_ > 0)
            cell_idx_--;
    }

    /// Move to the cell at right.
    void right_shift() noexcept {
        if (cell_idx_ < cells_.size())
            cell_idx_++;
    }

    /// Writes current cell into output stream.
    /// @param os The output stream to output to.
    void output(std::ostream& os) const {
        os << current_cell();
    }

    /// Runs the interpreter.
    /// @param os The output stream for writing output.
    /// @param is The input stream to input input from.
    void run(std::ostream& os, std::istream& is) {
        while (instruction_idx_ < instructions_.size()) {
            auto incIdx = true;
            switch (instructions_[instruction_idx_]) {
                case DecrementChar: decrement(); break;
                case IncrementChar: increment(); break;
                case InputCharChar: input(is); break;
                case LeftShiftChar: left_shift(); break;
                case LoopEndChar: instruction_idx_ = end_loop(); incIdx = false; break;
                case LoopStartChar: instruction_idx_ = start_loop(); incIdx = false; break;
                case OutputChar: output(os); break;
                case RightShiftChar: right_shift(); break;
            }
            if (incIdx)
                instruction_idx_++;
        }

        if (in_loop())
            throw std::runtime_error{"end of loop not found"};
    }

private:
    std::size_t cell_idx_ = 0;
    std::vector<char> cells_;
    std::size_t instruction_idx_ = 0;
    std::string instructions_;
    std::stack<std::size_t> loop_idxes_;

    /// Starts a loop and returns the index of next instruction.
    [[nodiscard]] std::size_t end_loop() {
        if (!in_loop())
            throw std::runtime_error{"loop did not start"};

        if (current_cell() == 0) {
            // loop counter is 0, quit
            loop_idxes_.pop();
            return instruction_idx_ + 1;
        }
        // loop counter is not 0, restart loop
        return loop_idxes_.top() + 1;
    }

    /// Whether we are currently in a loop.
    [[nodiscard]] bool in_loop() const noexcept {
        return !loop_idxes_.empty();
    }

    /// Finds the current loop ending index.
    [[nodiscard]] std::size_t loop_end_idx() const {
        if (!in_loop())
            throw std::runtime_error{"not in loop"};

        std::size_t openCount = 1;
        for (std::size_t i = instruction_idx_ + 1; i < instructions_.size(); i++) {
            if (instructions_[i] == LoopStartChar)
                openCount++;
            else if (instructions_[i] == LoopEndChar)
                openCount--;

            if (openCount == 0)
                return i;
        }
        throw std::runtime_error{"end of loop not found"};
    }

    /// Starts a loop and returns the index of next instruction.
    [[nodiscard]] std::size_t start_loop() {
        if (current_cell() == 0)    // don't run loop if loop counter is 0
            return loop_end_idx() + 1;

        loop_idxes_.push(instruction_idx_);
        return instruction_idx_ + 1;
    }
};

#endif  // BFINT_HPP
