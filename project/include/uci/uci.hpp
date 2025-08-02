#pragma once

#include <sstream>
#include <thread>

#include "app_state.hpp"

class UCI {
   public:
    void start(AppState& state) {
        std::cout.setf(std::ios::unitbuf);
        m_state  = &state;
        m_thread = std::thread([this] { runLoop(); });
    }

    void stop() {
        if (m_thread.joinable()) {
            m_thread.join();
        }
    }

   private:
    void runLoop() {
        std::string line;
        while (!m_state->shouldQuit() && std::getline(std::cin, line)) {
            if (line == "quit") {
                m_state->signalQuit();
                break;
            }
            processCommand(line);
        }
    }

    void processCommand(const std::string& command) {
        if (command == "uci") {
            std::cout << "uciok\n";
        } else if (command.starts_with("position")) {
            handlePosition(command);
        } else if (command.starts_with("go perft")) {
            handlePerft(command);
        }
    }

    void handlePosition(const std::string& command) {
        m_state->applyToAllGames([&](Game& game) {
            if (command.find("startpos") != std::string::npos) {
                game.start();
            } else if (command.find("fen") != std::string::npos) {
                // to parse fen
            }

            size_t movesPos = command.find("moves");
            if (movesPos != std::string::npos) {
                std::istringstream iss(command.substr(movesPos + 6));
                std::string        move;

                while (iss >> move) {
                    game.doMove(move);
                }
            }
        });
    }

    void handlePerft(const std::string& command) {
        int depth = std::stoi(command.substr(9));
        m_state->applyToAllGames([depth](Game& game) { game.perft(depth); });
    }

    AppState*   m_state = nullptr;
    std::thread m_thread;
};

/*
Just a shortcut, I have to find place for it in UCI
int testPerformance(Game& game, int depth, bool verbose = false) {
    auto start = std::chrono::high_resolution_clock::now();

    int nodes = Movegen::perft(game.getPosition(), depth, verbose);

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

    return nodes;
}*/