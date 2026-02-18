#include <exception>
#include <iostream>

#include "engine.hpp"
#include "uci.hpp"

int main() {
    try {
        Engine engine;

        Uci uci(engine);
        uci.run();
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << '\n';
        return 1;
    }
}
