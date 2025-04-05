#include "game.hpp"
#include "renderer.hpp"
#include "resource_manager.hpp"
#include "utils.hpp"

int main() {
    ResourceManager::loadResources();

    Game game;
    Renderer renderer;
    renderer.initialize(Dimensions(800, 600), "Kaban", true);
    renderer.hookUpGame(&game);

    game.setFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
    // game.countMoves(4);
    while (!renderer.windowShouldClose()) {
        renderer.render();
    }
    renderer.terminate();
}