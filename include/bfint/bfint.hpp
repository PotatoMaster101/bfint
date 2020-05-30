////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Simple Brainfuck interpretor written in C++17.
//
// Author: PotatoMaster101
// Date:   30/05/2020
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef BFINT_HPP
#define BFINT_HPP
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <stack>
#include <iterator>     // std::back_inserter
#include <algorithm>    // std::fill_n
#include <cstddef>      // std::size_t
#include <cstdint>      // std::uint8_t

// Brainfuck language token types.
enum class bftoken {
    inc,        // +
    dec,        // -
    lsh,        // <
    rsh,        // >
    out,        // .
    in,         // ,
    lstart,     // [
    lend,       // ]
};

// Brainfuck interpretor.
class bfint {
public:
    // Map of Brainfuck tokens.
    const std::map<char, bftoken> TOKEN_MAP {
        {'+', bftoken::inc},
        {'-', bftoken::dec},
        {'<', bftoken::lsh},
        {'>', bftoken::rsh},
        {'.', bftoken::out},
        {',', bftoken::in},
        {'[', bftoken::lstart},
        {']', bftoken::lend}
    };

    // Member types.
    using size_type = std::size_t;

    // Constructor.
    bfint(const std::string& str, size_type cells = 10000) :
        insts_{str}, cells_(cells, 0x0), instptr_{0}, cellptr_{0} {}

    // Rule of five.
    bfint(const bfint& rhs) = default;
    bfint& operator=(const bfint& rhs) = default;
    bfint(bfint&& rhs) noexcept = default;
    bfint& operator=(bfint&& rhs) noexcept = default;
    ~bfint() noexcept = default;

    // Returns the content of the current cell.
    inline std::uint8_t cell() const noexcept
    { return cells_[cellptr_]; }

    // Returns the content of the specified cell.
    inline std::uint8_t cell(size_type i) const noexcept
    { return cells_[i]; }

    // Increments the current cell and returns the new value.
    inline std::uint8_t inc() noexcept
    { cells_[cellptr_]++; return cells_[cellptr_]; }

    // Decrements the current cell and returns the new value.
    inline std::uint8_t dec() noexcept
    { cells_[cellptr_]--; return cells_[cellptr_]; }

    // Outputs the current cell content into the given stream.
    inline void output(std::ostream& os = std::cout) const
    { os << cell() << " "; }

    // Outputs the current cell content as a char into the given stream.
    inline void output_ch(std::ostream& os = std::cout) const
    { os << static_cast<char>(cell()); }

    // Inputs a char from the given stream and set as current cell.
    inline void input_ch(std::istream& is = std::cin)
    { char ch; is >> ch; cells_[cellptr_] = ch; }

    // Go to the next left cell. Does nothing if at the start.
    inline void lshift() noexcept
    { if (cellptr_ != 0) --cellptr_; }

    // Go to the next right cell. Grows the cell vector if at the end.
    inline void rshift()
    { if (cellptr_ >= (cells_.size() - 1)) growcell(); ++cellptr_; }

    // Indicates the start of a loop.
    //
    // EXCEPT:
    // std::runtime_error - when end of loop is not found
    size_type start_loop();

    // Indicates the end of a loop.
    //
    // EXCEPT:
    // std::runtime_error - when start of loop is not found
    size_type end_loop();

    // Runs the Brainfuck code.
    //
    // EXCEPT:
    // std::runtime_error - when shit happens
    void run();

private:
    std::string insts_;                 // instructions
    std::vector<std::uint8_t> cells_;   // list of cells
    size_type instptr_;                 // instruction pointer
    size_type cellptr_;                 // cell pointer
    std::stack<size_type> loopstk_;     // loop stack, contains starting index

    // Determines whether the current instruction is in a loop.
    inline bool inloop() const noexcept
    { return !(loopstk_.empty()); }

    // Grows the cell vector by the specified amount. New cells will be set to 0.
    inline void growcell(size_type n = 100)
    { std::fill_n(std::back_inserter(cells_), n, static_cast<uint8_t>(0)); }    // to fix warning on MSVC

    // Returns the instruction index of the end of current loop (the ] token).
    //
    // EXCEPT:
    // std::runtime_error - when end of loop is not found
    size_type find_loop_end() const;
};

#endif
