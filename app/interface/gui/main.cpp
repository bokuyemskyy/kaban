#include <exception>
#include <iostream>

#include "engine.hpp"
#include "gui.hpp"

int main() {
    try {
        constexpr int WIDTH              = 800;
        constexpr int HEIGHT             = 600;
        constexpr int MAX_SEARCH_TIME_MS = 5000;

        Engine engine(true, MAX_SEARCH_TIME_MS);
        Gui    gui(engine, WIDTH, HEIGHT, "Kaban", true);

        gui.run();
    } catch (const std::exception& e) {
        std::cerr << "Unhandled exception: " << e.what() << '\n';
        return 1;
    } catch (...) {
        std::cerr << "Unknown exception\n";
        return 1;
    }

    return 0;
}