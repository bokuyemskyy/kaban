#include "game.hpp"
#include "input_handler.hpp"
#include "renderer.hpp"

int main() {
    Game game;
    Renderer renderer;
    InputHandler inputHandler(&renderer, &game);
    renderer.init(600, "Kaban");
    renderer.hookUpGame(&game);

    game.setFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
    // game.countMoves(4);
    while (!renderer.windowShouldClose()) {
        inputHandler.update();
        renderer.render();
    }
    renderer.shutdown();
}