#include <unistd.h>

#include <chrono>
#include <iostream>
#include <thread>

#include "app_state.hpp"
#include "include/uci/uci.hpp"
#include "renderer.hpp"
#include "resource_manager.hpp"
#include "utils.hpp"

int main() {
    std::cout.setf(std::ios::unitbuf);

    ResourceManager::init();

    AppState state;

    Renderer renderer;
    UCI      uci;

    renderer.start(Dimensions(800, 600), "Kaban", true, state);
    uci.start(state);

    while (!state.shouldQuit()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    uci.stop();
    renderer.stop();

    return 0;
}
