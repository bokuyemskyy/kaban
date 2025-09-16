#pragma once

#include <unistd.h>

#include <cassert>
#include <deque>
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
        std::istringstream      iss(command);
        std::deque<std::string> tokens;
        std::string             token;
        while (iss >> token) tokens.push_back(token);
        if (tokens.empty()) return;

        const std::string& cmd = tokens.front();
        tokens.pop_front();

        if (cmd == "uci") {
            std::cout << "id name " << AppInfo::NAME << " " << AppInfo::VERSION << std::endl;
            std::cout << "id author " << AppInfo::AUTHOR << std::endl;
            std::cout << "uciok\n";
        } else if (cmd == "isready") {
            std::cout << "readyok" << std::endl;
        } else if (cmd == "ucinewgame") {
            m_engine.newGame();
        } else if (cmd == "position") {
            positions(tokens);
        } else if (cmd == "go") {
            go(tokens);
        } else {
            std::cerr << "Unknown command: \"" << command << "\"." << std::endl;
        }
    }

    void positions(std::deque<std::string>& tokens) {
        if (tokens.empty()) return;

        if (tokens.front() == "startpos") {
            m_engine.newGame();
            tokens.pop_front();
        } else if (tokens.front() == "fen") {
            tokens.pop_front();
            std::string fen;
            for (int i = 0; i < 6 && !tokens.empty(); ++i) {
                fen += tokens.front() + " ";
                tokens.pop_front();
            }
            m_engine.fromFen(fen);
        }

        if (!tokens.empty() && tokens.front() == "moves") {
            tokens.pop_front();
            while (!tokens.empty()) {
                m_engine.makeMove(tokens.front());
                tokens.pop_front();
            }
        }
    }

    void go(std::deque<std::string>& tokens) {
        if (tokens.empty()) return;

        if (tokens.front() == "perft") {
            tokens.pop_front();
            int depth = 1;
            if (!tokens.empty()) {
                depth = std::stoi(tokens.front());
                tokens.pop_front();
            }
            std::cout << "Running perft" << std::endl;
            m_engine.perft(depth);
        }
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