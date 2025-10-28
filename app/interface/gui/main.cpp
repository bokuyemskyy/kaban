#include <exception>
#include <iostream>

#include "engine.hpp"
#include "gui.hpp"

int main() {
    try {
        const int WIDTH  = 800;
        const int HEIGHT = 600;

        Engine engine;
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