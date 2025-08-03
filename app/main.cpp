#include "renderer.hpp"
#include "session.hpp"
#include "uci.hpp"

int main() {
    Session session;

    Renderer renderer;
    UCI      uci;

    renderer.start(800, 600, "Kaban", true, session);
    uci.start(session);

    while (!session.shouldQuit()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    renderer.stop();
    uci.stop();

    return 0;
}
