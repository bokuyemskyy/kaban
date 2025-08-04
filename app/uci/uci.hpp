#pragma once

#include <chrono>
#include <thread>

#include "session.hpp"

class UCI {
   public:
    void start(Session& session) {
        std::cout.setf(std::ios::unitbuf);
        m_session = &session;
        m_thread  = std::thread([this] { runLoop(); });
    }

    void stop() {
        if (m_thread.joinable()) {
            m_thread.join();
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

    void runLoop() {
        std::string line;
        while (!m_session->shouldQuit()) {
            if (inputAvailable()) {
                if (std::getline(std::cin, line)) {
                    if (line == "quit") {
                        m_session->signalQuit();
                        break;
                    }
                    processCommand(line);
                }
            } else {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
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
                    game.doMove(move);
                }
            }
        });*/
    }

    void handlePerft(const std::string& /*command*/) {
        // int depth = std::stoi(command.substr(9));
        // m_state->applyToCurrentGame([depth](Game& game) { game.perft(depth); });
    }

    Session*    m_session = nullptr;
    std::thread m_thread;
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