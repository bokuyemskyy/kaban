#pragma once

#include <algorithm>
#include <string>
#include <thread>
#include <vector>

#include "evaluation.hpp"
#include "history.hpp"
#include "move.hpp"
#include "position.hpp"
#include "square.hpp"

struct SearchParameters {
    int max_depth = -1;

    int max_time_ms = -1;

    int wtime_ms = -1;
    int btime_ms = -1;
};

class Engine {
   public:
    void        newGame() { m_position.fromFen(); }
    void        fromFen(const std::string& fen) { m_position.fromFen(fen); }
    std::string toFen() { return m_position.toFen(); }
    void        go(const SearchParameters& params) {
        m_stop_search = false;

        if (m_search_thread.joinable()) {
            m_search_thread.join();
        }

        SearchParameters effective_params = params;

        auto calculate_time_limit = [](int time_ms) -> int { return std::max(time_ms / 100, 2); };

        if (params.wtime_ms != -1 && m_position.us() == Colors::WHITE) {
            effective_params.max_time_ms = calculate_time_limit(params.wtime_ms);
        } else if (params.btime_ms != -1 && m_position.us() == Colors::BLACK) {
            effective_params.max_time_ms = calculate_time_limit(params.btime_ms);
        }

        m_search_thread = std::thread(&Engine::search, this, effective_params);
    }

    void search(const SearchParameters& params) {
        std::array<Move, 256> possible_moves{};
        size_t                size = m_position.generateMoves<GenerationTypes::LEGAL>(possible_moves.data());

        auto scoreMove = [&](const Move& m) {
            if (m_position.at(m.to()) != Pieces::NONE) {
                return 10 * Evaluation::pieceValue(m_position.at(m.to()).type()) -
                       Evaluation::pieceValue(m_position.at(m.from()).type());
            }
            return 0;
        };

        std::sort(possible_moves.begin(), possible_moves.begin() + size,
                  [&](const Move& a, const Move& b) { return scoreMove(a) > scoreMove(b); });

        if (size == 0) {
            m_best_move   = Move();
            m_stop_search = true;
            return;
        }

        auto start_time = std::chrono::steady_clock::now();

        int max_depth = (params.max_depth != -1) ? params.max_depth : 64;

        m_best_move = possible_moves[0];

        for (int depth = 1; depth <= max_depth; ++depth) {
            std::cout << "Depth " << depth << std::endl;
            if (params.max_time_ms != -1) {
                auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() -
                                                                                     start_time);

                std::cout << elapsed.count() << " " << params.max_time_ms << std::endl;
                if (elapsed.count() >= params.max_time_ms * 1000) {
                    std::cout << "a" << std::endl;
                    m_stop_search = true;
                }
            }
            std::cout << "b" << std::endl;

            double alpha               = -std::numeric_limits<double>::infinity();
            double beta                = std::numeric_limits<double>::infinity();
            double best_score_at_depth = -std::numeric_limits<double>::infinity();
            Move   current_best_move   = m_best_move;

            for (size_t i = 0; i < size; i++) {
                Move& move = possible_moves[i];
                if (m_stop_search) {
                    break;
                };
                auto undo_info = m_position.makeMove(move);

                double score = -minimax(m_position, depth - 1, -beta, -alpha);

                m_position.unmakeMove(move, undo_info);

                if (score > best_score_at_depth) {
                    best_score_at_depth = score;
                    current_best_move   = move;
                    std::cout << "Score: " << score << " for move " << move.toString() << std::endl;
                }

                alpha = std::max(alpha, best_score_at_depth);
            }

            if (!m_stop_search) {
                m_best_move = current_best_move;
            } else {
                break;
            }
        }
        if (m_best_move.hasValue()) {
            std::cout << "bestmove " << m_best_move.toString() << std::endl;
        }
        m_stop_search = true;
        m_best_move   = Move();
    }

    double minimax(Position& pos, int depth, double alpha, double beta) {
        if (m_stop_search) return 0.0;

        if (pos.isGameOver()) {
            if (pos.isCheckmate()) {
                return Evaluation::MATE_SCORE - depth;
            }
            if (pos.isStalemate()) {
                return 0.0;
            }
        }

        if (depth == 0) {
            return Evaluation::evaluatePosition(pos);
        }

        std::array<Move, 256> possible_moves{};
        size_t                size = pos.generateMoves<GenerationTypes::LEGAL>(possible_moves.data());

        auto scoreMove = [&](const Move& m) {
            if (m_position.at(m.to()) != Pieces::NONE) {
                return 10 * Evaluation::pieceValue(m_position.at(m.to()).type()) -
                       Evaluation::pieceValue(m_position.at(m.from()).type());
            }
            return 0;
        };

        std::sort(possible_moves.begin(), possible_moves.begin() + size,
                  [&](const Move& a, const Move& b) { return scoreMove(a) > scoreMove(b); });

        double best_score = -std::numeric_limits<double>::infinity();

        for (size_t i = 0; i < size; i++) {
            Move& move      = possible_moves[i];
            auto  undo_info = pos.makeMove(move);

            double score = -minimax(pos, depth - 1, -beta, -alpha);

            pos.unmakeMove(move, undo_info);

            best_score = std::max(best_score, score);

            alpha = std::max(alpha, best_score);
            if (alpha >= beta) {
                break;
            }
        }

        return best_score;
    }

    void stop() {
        m_stop_search = true;

        if (m_search_thread.joinable()) {
            m_search_thread.join();
        }
    }

    uint64_t perft(int depth) {
        if (depth > 64) throw std::runtime_error("The depth cant be > 64");
        static std::array<Move, 256 * 64> move_stack{};

        using Clock = std::chrono::high_resolution_clock;
        auto start  = Clock::now();

        uint64_t total = perft<true>(depth, move_stack.data());

        auto                          end     = Clock::now();
        std::chrono::duration<double> elapsed = end - start;

        std::cout << "Total nodes: " << total << "\n";
        std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";
        std::cout << "Nodes per second: " << std::floor(total / elapsed.count()) << "\n";

        return total;
    }
    bool isLegal() { return m_position.isLegal<false>(); }

    [[nodiscard]] auto              board() const { return m_position.board(); }
    [[nodiscard]] auto              at(Square square) { return m_position.at(square); }
    [[nodiscard]] std::vector<Move> moves() {
        std::array<Move, 256> move_list{};
        size_t                size = m_position.generateMoves<GenerationTypes::LEGAL>(move_list.data());
        std::vector<Move>     result(move_list.begin(), move_list.begin() + size);
        return result;
    }

    void makeMove(Move move) {
        std::array<Move, 256> move_list{};
        size_t                size = m_position.generateMoves<GenerationTypes::LEGAL>(move_list.data());
        auto                  it   = std::ranges::find_if(move_list.begin(), move_list.begin() + size,
                                                          [&](const Move& m) { return m.from() == move.from() && m.to() == move.to(); });

        if (it == move_list.begin() + size) throw std::runtime_error("Invalid move");

        auto undo_info = m_position.makeMove(*it);
        m_history.push(*it, undo_info);
    }

    void makeMove(const std::string& move) {
        if (move.size() != 4 && move.size() != 5) throw std::runtime_error("Invalid move format");

        std::array<Move, 256> move_list{};
        size_t                size = m_position.generateMoves<GenerationTypes::LEGAL>(move_list.data());

        Move target = Move::fromString(move);

        if (move.size() == 4) {
            auto it = std::ranges::find_if(move_list.begin(), move_list.begin() + size, [&](const Move& m) {
                return m.from() == target.from() && m.to() == target.to();
            });

            if (it == move_list.begin() + size) throw std::runtime_error("Invalid move");

            auto undo_info = m_position.makeMove(*it);
            m_history.push(*it, undo_info);
        } else {
            auto it = std::ranges::find_if(move_list.begin(), move_list.begin() + size, [&](const Move& m) {
                return m.from() == target.from() && m.to() == target.to() && m.flag() == target.flag();
            });

            if (it == move_list.begin() + size) throw std::runtime_error("Invalid move");

            auto undo_info = m_position.makeMove(*it);
            m_history.push(*it, undo_info);
        }
    }

    void unmakeMove() {
        auto [move, undo_info] = m_history.pop();
        m_position.unmakeMove(move, undo_info);
    }

   private:
    Position m_position{};

    History m_history{};

    template <bool Root>
    uint64_t perft(int depth, Move* move_list) {
        if (depth == 0) return 1;

        size_t   size  = m_position.generateMoves<GenerationTypes::ALL>(move_list);
        uint64_t nodes = 0;

        for (size_t i = 0; i < size; ++i) {
            UndoInfo undo = m_position.makeMove(move_list[i]);
            if (m_position.isLegal<false>()) [[likely]] {
                uint64_t child_nodes = perft<false>(depth - 1, move_list + size);

                if constexpr (Root) {
                    std::cout << move_list[i].toString() << ": " << child_nodes << '\n';
                }

                nodes += child_nodes;
            }
            m_position.unmakeMove(move_list[i], undo);
        }

        return nodes;
    }

    Move              m_best_move = Move();
    std::thread       m_search_thread;
    std::atomic<bool> m_stop_search = false;
};