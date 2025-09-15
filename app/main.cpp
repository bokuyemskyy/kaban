#include <string>

#include "gui.hpp"
#include "uci.hpp"

int main(int argc, char** argv) {
    Engine engine;
    if (argc > 1 && std::string(argv[1]) == "uci") {
        Uci uci(engine);
        uci.run();
    } else {
        Gui gui(engine, 800, 600, "Kaban", true);
        gui.run();
    }
}