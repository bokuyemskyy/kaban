#pragma once

#include <unistd.h>

#include <cassert>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

#include "app_info.hpp"
#include "engine.hpp"

class Uci {
   public:
    Uci(Engine& engine) : m_engine(engine) { std::cout.setf(std::ios::unitbuf); }

    void run() {
        std::string line;

        std::cout << AppInfo::NAME << " " << AppInfo::VERSION << " is running!" << std::endl;

        while (!m_shouldQuit) {
            if (inputAvailable()) {
                if (std::getline(std::cin, line)) {
                    if (line == "quit") {
                        m_shouldQuit = true;
                        break;
                    }
                    processCommand(line);
                }
            } else {
                usleep(10000);
            }
        }
    }

   private:
    static bool inputAvailable() {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);
        timeval timeout = {.tv_sec = 0, .tv_usec = 0};
        return select(STDIN_FILENO + 1, &readfds, nullptr, nullptr, &timeout) > 0;
    }

    void processCommand(const std::string& command) {
        std::istringstream       iss(command);
        std::vector<std::string> tokens;
        std::string              token;
        while (iss >> token) tokens.push_back(token);

        if (tokens[0] == "uci") {
            std::cout << "id name " << AppInfo::NAME << " " << AppInfo::VERSION << std::endl;
            std::cout << "id author " << AppInfo::AUTHOR << std::endl;
            std::cout << "uciok\n";
        } else if (tokens[0] == "isready") {
            std::cout << "readyok" << std::endl;
        } else if (tokens[0] == "ucinewgame") {
            m_engine.newGame();
        } else if (tokens[0] == "position") {
            handlePosition(command);
        } else if (tokens[0] == "go") {
            handlePerft(command);
        } else {
            std::cout << "Unknown command: \"" << command << "\"." << std::endl;
        }
    }

    void handlePosition(const std::string& /*command*/) {
        /*m_session->applyToAllGames([&](Game& game) {
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
                    game.do_move(move);
                }
            }
        });*/
    }

    void handlePerft(const std::string& /*command*/) {
        // int depth = std::stoi(command.substr(9));
        // m_state->applyToCurrentGame([depth](Game& game) { game.perft(depth); });
    }

    bool m_shouldQuit{false};

    Engine& m_engine;
};

/*
Just a piece of code I have used, I have to find place for it in UCI
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