#include "game.hpp"
#include "input_handler.hpp"
#include "renderer.hpp"
#include "resource_manager.hpp"
#include "utils.hpp"

int main() {
    ResourceManager::loadResources();

    Game game;
    Renderer renderer;
    InputHandler inputHandler(&renderer, &game);
    renderer.initialize(Dimensions(800, 600), "Kaban", true);
    renderer.hookUpGame(&game);

    game.setFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
    // game.countMoves(4);
    while (!renderer.windowShouldClose()) {
        inputHandler.update();
        renderer.render();
    }
    renderer.terminate();
}