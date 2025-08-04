#include <boost/process.hpp>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <regex>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

///
///
/// BOOST NO LONGER HAS IPSTREAM/OPSTREAM. NEEDS REFACTOR
///
///

/*namespace bp = boost::process;

std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w";
int         depth;

bp::ipstream stockfishOut, kabanOut;
bp::opstream stockfishIn, kabanIn;

std::mutex              sfMutex, kbMutex;
std::condition_variable sfCv, kbCv;
std::queue<std::string> sfLines, kbLines;

void readEngineOutput(bp::ipstream& out, std::queue<std::string>& lines, std::mutex& mtx, std::condition_variable& cv) {
    std::string line;
    while (std::getline(out, line)) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            lines.push(line);
        }
        cv.notify_one();
    }
}

template <typename Predicate>
std::vector<std::string> waitForOutput(std::queue<std::string>& lines, std::mutex& mtx, std::condition_variable& cv,
                                       Predicate stopCondition) {
    std::vector<std::string> collected;
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [&] { return !lines.empty(); });
        auto line = lines.front();
        lines.pop();
        lock.unlock();

        collected.push_back(line);

        if (stopCondition(line)) break;
    }
    return collected;
}

std::unordered_map<std::string, int> parsePerftOutput(const std::vector<std::string>& lines) {
    std::unordered_map<std::string, int> moveMap;
    std::regex                           moveRegex(R"(([a-h][1-8][a-h][1-8][nbrq]?): (\d+))");

    for (const auto& line : lines) {
        auto begin = std::sregex_iterator(line.begin(), line.end(), moveRegex);
        auto end   = std::sregex_iterator();

        for (auto it = begin; it != end; ++it) {
            std::string move  = (*it)[1];
            int         nodes = std::stoi((*it)[2]);
            moveMap[move]     = nodes;
        }
    }
    return moveMap;
}

void initializeEngine(bp::opstream& in, std::queue<std::string>& lines, std::mutex& mtx, std::condition_variable& cv,
                      const std::string& tag) {
    in << "uci\n" << std::flush;
    auto uciLines = waitForOutput(lines, mtx, cv, [](const std::string& l) { return l == "uciok"; });
}

std::unordered_map<std::string, int> runPerft(bp::opstream& in, std::queue<std::string>& lines, std::mutex& mtx,
                                              std::condition_variable& cv, const std::string& fen, int depth,
                                              const std::vector<std::string>& moves, const std::string& tag) {
    in << "position fen " << fen;
    if (!moves.empty()) {
        in << " moves";
        for (auto& m : moves) {
            in << " " << m;
        }
    }
    in << "\n";
    in << "go perft " << depth << "\n" << std::flush;

    auto outputLines = waitForOutput(lines, mtx, cv, [depth](const std::string& l) {
        return l.find("Perft(" + std::to_string(depth) + "):") == 0 || l.find("Nodes") == 0;
    });

    return parsePerftOutput(outputLines);
}

void reportMismatch(std::string faultyMove, bool odd, std::vector<std::string> trace = {}) {
    std::cout << "Mismatch found!\nTrace: \nposition fen " << fen;
    if (trace.size() != 0) {
        std::cout << " moves";
        for (auto move : trace) {
            std::cout << " " << move;
        }
        std::cout << "\n";
    }
    std::cout << "go perft 1\n";
    if (odd) {
        std::cout << "[ODD] ";
    } else {
        std::cout << "[MISSING] ";
    }
    std::cout << "Faulty move: " << faultyMove << "\n";
}

bool comparePerftRecursive(int depthLeft, std::vector<std::string> trace = {}) {
    auto sfPerft = runPerft(stockfishIn, sfLines, sfMutex, sfCv, fen, depthLeft, trace, "SF");
    auto kbPerft = runPerft(kabanIn, kbLines, kbMutex, kbCv, fen, depthLeft, trace, "KB");

    if (sfPerft.size() > kbPerft.size()) {
        std::cout << "SF: \n";
        for (const auto& [move, nodes] : sfPerft) {
            std::cout << move << ": " << nodes << "\n";
        }
        std::cout << "KB: \n";
        for (const auto& [move, nodes] : kbPerft) {
            std::cout << move << ": " << nodes << "\n";
        }

        for (const auto& [move, nodes] : sfPerft) {
            if (kbPerft.count(move) == 0) {
                reportMismatch(move, false, trace);
                return true;
            }
        }
    } else if (sfPerft.size() < kbPerft.size()) {
        for (const auto& [move, nodes] : kbPerft) {
            if (sfPerft.count(move) == 0) {
                reportMismatch(move, true, trace);
                return true;
            }
        }
    } else {
        for (const auto& [move, nodes] : sfPerft) {
            int kbNodes = kbPerft.at(move);
            if (kbNodes != nodes) {
                trace.push_back(move);
                return comparePerftRecursive(depthLeft - 1, trace);
            }
        }
    }
    return false;
}

int main(int argc, char* argv[]) {
    if (argc != 2 && argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <DEPTH> <FEN>(optional)\n";
        return 1;
    }
    try {
        depth = std::stoi(argv[1]);
    } catch (...) {
        std::cerr << "Invalid depth\n";
        return 1;
    }
    if (argc == 3) fen = argv[2];

    bp::child stockfish("/usr/bin/stockfish", bp::std_out > stockfishOut, bp::std_in < stockfishIn);
    bp::child kaban("/home/amon/Code/kaban/out/bin/debug/main", bp::std_out > kabanOut, bp::std_in < kabanIn);

    std::thread sfReader(readEngineOutput, std::ref(stockfishOut), std::ref(sfLines), std::ref(sfMutex),
                         std::ref(sfCv));
    std::thread kbReader(readEngineOutput, std::ref(kabanOut), std::ref(kbLines), std::ref(kbMutex), std::ref(kbCv));

    initializeEngine(stockfishIn, sfLines, sfMutex, sfCv, "SF");
    initializeEngine(kabanIn, kbLines, kbMutex, kbCv, "KB");

    bool mismatchFound = comparePerftRecursive(depth);

    if (!mismatchFound) {
        std::cout << "Perft results match up to depth " << depth << ".\n";
    }

    stockfishIn << "quit\n" << std::flush;
    kabanIn << "quit\n" << std::flush;

    stockfish.wait();
    kaban.wait();

    sfReader.join();
    kbReader.join();

    return 0;
}*/

int main() {}