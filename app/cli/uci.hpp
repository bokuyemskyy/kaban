#pragma once

#include <unistd.h>

#include <chrono>
#include <iostream>

class Uci {
   public:
    Uci() { std::cout.setf(std::ios::unitbuf); }

    void run() {
        std::string line;
        while (!m_shouldQuit) {
            if (input_available()) {
                if (std::getline(std::cin, line)) {
                    if (line == "quit") {
                        m_shouldQuit = true;
                        break;
                    }
                    process_command(line);
                }
            } else {
                usleep(10000);
            }
        }
    }

   private:
    static bool input_available() {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);
        timeval timeout = {.tv_sec = 0, .tv_usec = 0};
        return select(STDIN_FILENO + 1, &readfds, nullptr, nullptr, &timeout) > 0;
    }

    void process_command(const std::string& command) {
        if (command == "uci") {
            std::cout << "uciok\n";
        } else if (command.starts_with("position")) {
            handle_position(command);
        } else if (command.starts_with("go perft")) {
            handle_perft(command);
        }
    }

    void handle_position(const std::string& /*command*/) {
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

    void handle_perft(const std::string& /*command*/) {
        // int depth = std::stoi(command.substr(9));
        // m_state->applyToCurrentGame([depth](Game& game) { game.perft(depth); });
    }

    bool m_shouldQuit{false};
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