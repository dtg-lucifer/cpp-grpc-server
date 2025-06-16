#include <exception>
#include <iostream>

#include "config/config.hpp"

int main() {
    Config config;

    try {
        config = Config::New();
    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
