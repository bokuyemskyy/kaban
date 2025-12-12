#pragma once

#include <cassert>
#include <deque>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>

#include "app_info.hpp"
#include "engine.hpp"

class Uci {
   public:
    explicit Uci(Engine& engine) : m_engine(engine) { std::cout.setf(std::ios::unitbuf); }

    void run() {
        std::string line;

        std::cout << AppInfo::NAME << " " << AppInfo::VERSION << " is running!" << std::endl;

        while (!m_shouldQuit && std::getline(std::cin, line)) {
            if (line.empty()) continue;
            processCommand(line);
        }
    }

   private:
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
            std::cout << "uciok" << std::endl;
        } else if (cmd == "isready") {
            std::cout << "readyok" << std::endl;
        } else if (cmd == "ucinewgame") {
            m_engine.newGame();
        } else if (cmd == "position") {
            setPosition(tokens);
        } else if (cmd == "go") {
            go(tokens);
        } else if (cmd == "fen") {
            std::cout << m_engine.toFen() << std::endl;
        } else if (cmd == "stop") {
            stop();
        } else if (cmd == "quit") {
            m_shouldQuit = true;
        } else {
            std::cout << "Unknown command: " << command << std::endl;
        }
    }

    void setPosition(std::deque<std::string>& tokens) {
        if (tokens.empty()) return;

        if (tokens.front() == "startpos") {
            m_engine.newGame();
            tokens.pop_front();
        } else if (tokens.front() == "fen") {
            tokens.pop_front();
            std::string fen;
            while (!tokens.empty() && tokens.front() != "moves") {
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
        if (tokens.empty()) m_engine.go(SearchParameters{});

        if (tokens.front() == "perft") {
            tokens.pop_front();
            int depth = 1;
            if (!tokens.empty()) {
                try {
                    depth = std::stoi(tokens.front());
                    tokens.pop_front();
                } catch (const std::exception& e) {
                    std::cerr << "Error: " << e.what() << std::endl;
                    return;
                }
            }

            auto nodes = m_engine.perft(depth);

            std::cout << std::endl;
            std::cout << "Nodes searched: " << nodes << std::endl;
        }

        SearchParameters params;

        while (!tokens.empty()) {
            std::string token = tokens.front();
            tokens.pop_front();

            try {
                if (token == "wtime" && !tokens.empty()) {
                    params.wtime_ms = std::stoi(tokens.front());
                    tokens.pop_front();
                } else if (token == "btime" && !tokens.empty()) {
                    params.btime_ms = std::stoi(tokens.front());
                    tokens.pop_front();
                } else if (token == "movetime" && !tokens.empty()) {
                    params.max_time_ms = std::stoi(tokens.front());
                    tokens.pop_front();
                } else if (token == "depth" && !tokens.empty()) {
                    params.max_depth = std::stoi(tokens.front());
                    tokens.pop_front();
                }

            } catch (const std::exception& e) {
                std::cerr << "Error parsing search parameter: " << token << std::endl;
                break;
            }
        }

        m_engine.go(params);
    }

    void stop() { m_engine.stop(); }

    bool m_shouldQuit{false};

    Engine& m_engine;
};
