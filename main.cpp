// https://github.com/PotatoMaster101/bfint

#include <fstream>
#include <iostream>
#include <sstream>
#include "bfint.hpp"

int main(int argc, const char *argv[]) {
    if (argc == 1) {
        std::cerr << "filename not provided\n";
        return 1;
    }

    std::ifstream file{argv[1]};
    if (!file) {
        std::cerr << "invalid file\n";
        return 1;
    }

    std::stringstream content;
    content << file.rdbuf();
    bfint interpreter{content.str()};
    try {
        interpreter.run(std::cout, std::cin);
        return 0;
    } catch (std::runtime_error& err) {
        std::cerr << interpreter.current_position() << ": " << err.what() << '\n';
        return 1;
    }
}
