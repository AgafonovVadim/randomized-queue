#include <cstdlib>
#include <iostream>

#include "Subset.hpp"

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <number of random strings printed>" << std::endl;
        return -1;
    }
    char* end;
    unsigned long k = std::strtoul(argv[1], &end, 10);
    if (*end != '\0') {
        std::cerr << "Incorrect number of strings to be printed\nUsage: " << argv[0]
                  << " <number of random strings printed>" << std::endl;
        return -1;
    }
    subset(k, std::cin, std::cout);
}
