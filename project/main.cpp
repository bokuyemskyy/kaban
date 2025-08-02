
#include "app_state.hpp"
#include "include/uci/uci.hpp"
#include "renderer.hpp"
#include "utils.hpp"

int main() {
    AppState app_state;

    Renderer renderer;
    UCI      uci;

    renderer.start(Dimensions(800, 600), "Kaban", true, app_state);
    uci.start(app_state);

    while (!app_state.shouldQuit()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    uci.stop();
    renderer.stop();

    return 0;
}
