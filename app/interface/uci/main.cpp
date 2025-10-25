#include "uci.hpp"

int main() {
    Engine engine;
    Uci    uci(engine);

    uci.run();
}
