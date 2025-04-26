#include <chrono>
#include <fstream>
#include <iostream>
#include <vector>

#include "game.hpp"
#include "renderer.hpp"
#include "resource_manager.hpp"
#include "utils.hpp"

void logPerformance(double nodesPerSec) {
    std::time_t now = std::time(nullptr);

    std::ofstream file("/home/amon/Desktop/stats.csv", std::ios::app);
    if (file.is_open()) {
        file << now << "," << std::fixed << std::setprecision(2) << nodesPerSec << "\n";
    }
}

int testPerformance(Game& game, int depth, bool verbose = false) {
    auto start = std::chrono::high_resolution_clock::now();

    int nodes = game.position().perft(depth, verbose);

    auto                          end     = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    double seconds     = elapsed.count();
    double nodesPerSec = nodes / seconds;
    double secPerNode  = seconds / nodes;

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Perft(" << depth << "): " << nodes << " nodes\n";
    std::cout << "Time: " << seconds << " sec\n";
    std::cout << "Speed: " << nodesPerSec << " nodes/sec\n";
    std::cout << "Efficiency: " << secPerNode << " sec/node\n";

    logPerformance(nodesPerSec);

    return nodes;
}

int main() {
    ResourceManager::loadResources();

    std::vector<Game> games;
    games.emplace_back();

    Renderer renderer;
    renderer.initialize(Dimensions(800, 600), "Kaban", true);
    renderer.attachGames(games);

    games[0].position().setFromFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w");
    testPerformance(games[0], 4, true);

    while (!renderer.windowShouldClose()) {
        renderer.render();
    }
    renderer.terminate();
}