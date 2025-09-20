#include <boost/asio.hpp>
#include <boost/asio/impl/read_until.hpp>
#include <boost/asio/writable_pipe.hpp>
#include <boost/process.hpp>
#include <boost/process/v2/stdio.hpp>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <regex>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

// REF_ = reference engine process. the engine you trust to be correct
// ENG_ = the engine that is being examined

namespace bp   = boost::process::v2;
namespace asio = boost::asio;

void sendCommand(asio::writable_pipe& in, const std::string& command) { asio::write(in, asio::buffer(command)); }

std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w";
int         depth;

asio::io_context ctx;

asio::writable_pipe ref_in(ctx);
asio::readable_pipe ref_out(ctx);

asio::writable_pipe eng_in(ctx);
asio::readable_pipe eng_out(ctx);

std::mutex              ref_mutex, eng_mutex;
std::condition_variable ref_cv, eng_cv;
std::queue<std::string> ref_lines, eng_lines;
void                    readEngineOutput(asio::readable_pipe& pipe, std::queue<std::string>& lines, std::mutex& mtx,
                                         std::condition_variable& cv) {
    asio::streambuf           buffer;
    std::istream              is(&buffer);
    boost::system::error_code ec;

    while (true) {
        std::size_t n = asio::read_until(pipe, buffer, '\n', ec);
        if (ec) break;

        std::string line;
        std::getline(is, line);
        if (line.empty()) continue;
        {
            std::lock_guard lock(mtx);
            lines.push(line);
        }
        cv.notify_one();
    }
}

template <typename Predicate>
std::vector<std::string> waitForOutput(std::queue<std::string>& lines, std::mutex& mtx, std::condition_variable& cv,
                                       Predicate stop) {
    std::vector<std::string> collected;
    while (true) {
        std::unique_lock lock(mtx);
        cv.wait(lock, [&] { return !lines.empty(); });

        std::string line = lines.front();
        lines.pop();
        lock.unlock();

        collected.push_back(line);
        if (stop(line)) break;
    }
    return collected;
}

std::unordered_map<std::string, int> parsePerft(const std::vector<std::string>& lines) {
    std::unordered_map<std::string, int> move_nodes;
    std::regex                           move_regex(R"(([a-h][1-8][a-h][1-8][nbrq]?): (\d+))");

    for (const auto& line : lines) {
        auto begin = std::sregex_iterator(line.begin(), line.end(), move_regex);
        auto end   = std::sregex_iterator();

        for (auto it = begin; it != end; ++it) {
            move_nodes[(*it)[1]] = std::stoi((*it)[2]);
        }
    }
    return move_nodes;
}

void initializeEngine(asio::writable_pipe& in, std::queue<std::string>& lines, std::mutex& mtx,
                      std::condition_variable& cv) {
    sendCommand(in, "uci\n");
    waitForOutput(lines, mtx, cv, [](const std::string& l) { return l == "uciok"; });
}

std::unordered_map<std::string, int> perft(asio::writable_pipe& in, std::queue<std::string>& lines, std::mutex& mtx,
                                           std::condition_variable& cv, const std::string& fen, int depth,
                                           const std::string& moves = "") {
    std::string cmd = "position fen " + fen;
    if (!moves.empty()) {
        cmd += " moves ";
        cmd += moves;
    }
    cmd += "\n";
    cmd += "go perft " + std::to_string(depth) + "\n";

    sendCommand(in, cmd);

    auto perft_output = waitForOutput(lines, mtx, cv, [depth](const std::string& l) { return l.find("Nodes") == 0; });

    return parsePerft(perft_output);
}

void mismatch(const std::string& move, bool odd, const std::string& trace = "") {
    std::cout << "Mismatch found!\nTrace: \nposition fen " << fen;
    if (!trace.empty()) {
        std::cout << "moves ";
        std::cout << trace;
    }
    std::cout << "go perft 1\n";
    std::cout << (odd ? "[ODD] " : "[MISSING] ") << "Faulty move: " << move << "\n";
}

bool comparePerft(int depth, const std::string& trace = "") {
    auto ref_perft = perft(ref_in, ref_lines, ref_mutex, ref_cv, fen, depth, trace);
    auto eng_perft = perft(eng_in, eng_lines, eng_mutex, eng_cv, fen, depth, trace);

    if (ref_perft.size() > eng_perft.size()) {
        for (auto& [move, nodes] : ref_perft)
            if (!eng_perft.contains(move)) {
                mismatch(move, false, trace);
                return true;
            }
    } else if (ref_perft.size() < eng_perft.size()) {
        for (auto& [move, nodes] : eng_perft)
            if (!ref_perft.contains(move)) {
                mismatch(move, true, trace);
                return true;
            }
    } else {
        for (auto& [move, ref_nodes] : ref_perft) {
            int eng_nodes = eng_perft.at(move);
            if (eng_nodes != ref_nodes) {
                return comparePerft(depth - 1, (trace + move + " "));
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

    std::string ref_path;
    std::cout << "Enter the reference engine path (correct one): ";
    std::cin >> ref_path;
    std::string eng_path;
    std::cout << "Enter your engine path (the one to test): ";
    std::cin >> eng_path;

    bp::process ref_process(ctx, ref_path, std::vector<std::string>{},
                            bp::process_stdio{.in = ref_in, .out = ref_out, .err = nullptr});
    bp::process eng_process(ctx, eng_path, std::vector<std::string>{"uci"},
                            bp::process_stdio{.in = eng_in, .out = eng_out, .err = nullptr});

    std::thread ref_reader(readEngineOutput, std::ref(ref_out), std::ref(ref_lines), std::ref(ref_mutex),
                           std::ref(ref_cv));
    std::thread eng_reader(readEngineOutput, std::ref(eng_out), std::ref(eng_lines), std::ref(eng_mutex),
                           std::ref(eng_cv));

    initializeEngine(ref_in, ref_lines, ref_mutex, ref_cv);
    initializeEngine(eng_in, eng_lines, eng_mutex, eng_cv);

    bool mismatch = comparePerft(depth);

    if (!mismatch) std::cout << "Perft results match up to depth " << depth << ".\n";

    sendCommand(ref_in, "quit\n");
    sendCommand(eng_in, "quit\n");

    ref_reader.join();
    eng_reader.join();

    ref_process.wait();
    eng_process.wait();

    return 0;
}
