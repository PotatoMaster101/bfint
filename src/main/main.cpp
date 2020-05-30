#include <iostream>
#include <fstream>
#include <sstream>
#include "bfint.hpp"

int main(int argc, const char *argv[]) {
    if (argc == 1) {
        std::cerr << "File name not provided." << std::endl;
        return 1;
    }

    std::ifstream file{argv[1]};
    if (file) {
        std::stringstream buff;
        buff << file.rdbuf();
        file.close();

        bfint bf{buff.str()};
        bf.run();
    }
}
