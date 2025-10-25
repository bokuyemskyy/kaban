#include "gui.hpp"

int main() {
    Engine engine;
    Gui    gui(engine, 800, 600, "Kaban", true);
    gui.run();
}