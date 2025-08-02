#include "app_state.hpp"
#include "renderer.hpp"
#include "uci.hpp"

int main() {
    AppState app_state;

    Renderer renderer;
    UCI      uci;

    renderer.start(800, 600, "Kaban", true, app_state);
    uci.start(app_state);

    while (!app_state.shouldQuit()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    renderer.stop();
    uci.stop();

    return 0;
}
