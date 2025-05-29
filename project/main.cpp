#include <unistd.h>

#include <chrono>
#include <csignal>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "game.hpp"
#include "init.hpp"
#include "renderer.hpp"
#include "resource_manager.hpp"
#include "utils.hpp"

std::mutex               moveMutex;
std::vector<std::string> pendingMoves;
std::atomic<bool>        useStartPos{false};
std::mutex               fenMutex;
std::string              currentFEN;
std::atomic<int>         perftRequest{-1};
std::atomic<bool>        quitFlag{false};

void logPerformance(double nodesPerSec) {
    std::time_t now = std::time(nullptr);

    std::ofstream file("/home/amon/Desktop/stats.csv", std::ios::app);
    if (file.is_open()) {
        file << now << "," << std::fixed << std::setprecision(2) << nodesPerSec << "\n";
    }
}

int testPerformance(Game& game, int depth, bool verbose = false) {
    auto start = std::chrono::high_resolution_clock::now();

    int nodes;
    if (verbose) {
        nodes = game.getPosition().perft<true>(depth);
    } else {
        nodes = game.getPosition().perft<false>(depth);
    }

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

// this whole shit was written by ai
// rewrite and create uci module asap
void cliLoop() {
    std::string                line;
    constexpr std::string_view kFenPrefix   = "position fen ";
    constexpr std::string_view kStartPrefix = "position startpos";
    constexpr std::string_view kMovesTag    = " moves ";

    while (std::getline(std::cin, line)) {
        if (line.rfind("uci", 0) == 0) {
            std::cout << "uciok\n";
        }
        if (line.rfind(kFenPrefix, 0) == 0) {
            size_t fenBeg   = kFenPrefix.size();  // first char of FEN
            size_t movesPos = line.find(kMovesTag, fenBeg);

            {  // save FEN
                std::lock_guard<std::mutex> lock(fenMutex);
                if (movesPos == std::string::npos)
                    currentFEN = line.substr(fenBeg);  // FEN only
                else
                    currentFEN = line.substr(fenBeg, movesPos - fenBeg);
            }

            {  // save moves if present
                std::lock_guard<std::mutex> lock(moveMutex);
                pendingMoves.clear();
                if (movesPos != std::string::npos) {
                    std::istringstream iss(line.substr(movesPos + kMovesTag.size()));
                    for (std::string m; iss >> m;) pendingMoves.push_back(m);
                }
            }
            useStartPos.store(false);
        }

        else if (line.rfind(kStartPrefix, 0) == 0) {
            size_t movesPos = line.find(kMovesTag);

            {  // save moves
                std::lock_guard<std::mutex> lock(moveMutex);
                pendingMoves.clear();
                if (movesPos != std::string::npos) {
                    std::istringstream iss(line.substr(movesPos + kMovesTag.size()));
                    for (std::string m; iss >> m;) pendingMoves.push_back(m);
                }
            }
            useStartPos.store(true);
        }

        else if (line.rfind("go perft ", 0) == 0 && line.size() > 9) {
            try {
                int depth = std::stoi(line.substr(9));
                if (depth >= 0) perftRequest.store(depth);
            } catch (...) {
                std::cout << "Bad depth value\n";
            }
        } else if (line == "quit") {
            quitFlag.store(true);
            break;
        }
    }
}

int main() {
    std::cout.setf(std::ios::unitbuf);
    std::thread cliThread(cliLoop);

    initEngine();                      // will be Engine::init()
    ResourceManager::loadResources();  // should be ResourceManager::init()

    std::vector<Game> games;
    games.emplace_back();

    Renderer renderer;  // should be init()
    renderer.initialize(Dimensions(800, 600), "Kaban", true);
    renderer.attachGames(games);

    std::this_thread::sleep_for(std::chrono::seconds(3));

    // testPerformance(games[0], 4, true);
    while (!renderer.windowShouldClose()) {
        {
            std::lock_guard<std::mutex> lock(fenMutex);
            if (!currentFEN.empty()) {
                games[0].getPosition().setFromFEN(currentFEN);
                currentFEN.clear();

                // Also apply pending moves if any
                std::vector<std::string> movesCopy;
                {
                    std::lock_guard<std::mutex> lock2(moveMutex);
                    movesCopy.swap(pendingMoves);
                }
                for (const auto& m : movesCopy) games[0].getPosition().makeMoveUci(m);

                useStartPos.store(false);
            }
        }

        // ----- position startpos + moves -----
        if (useStartPos.exchange(false)) {
            games[0].getPosition().setFromFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
            std::vector<std::string> movesCopy;
            {
                std::lock_guard<std::mutex> lock(moveMutex);
                movesCopy.swap(pendingMoves);
            }
            for (const auto& m : movesCopy) games[0].getPosition().makeMoveUci(m);
        }

        // ----- go perft N -----
        int depth = perftRequest.exchange(-1);
        if (depth >= 0) {
            testPerformance(games[0], depth, true);
        }
        if (quitFlag.load()) break;
        renderer.render();
    }
    renderer.terminate();
    cliThread.join();
    return 1;
}
