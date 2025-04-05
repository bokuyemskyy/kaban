#include <vector>

#include "game.hpp"
#include "renderer.hpp"
#include "resource_manager.hpp"
#include "utils.hpp"

int main() {
    ResourceManager::loadResources();

    std::vector<Game> games;
    games.emplace_back();

    Renderer renderer;
    renderer.initialize(Dimensions(800, 600), "Kaban", true);
    renderer.attachGames(games);

    // game.countMoves(4);
    while (!renderer.windowShouldClose()) {
        renderer.render();
    }
    renderer.terminate();
}