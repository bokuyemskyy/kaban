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
        // 1. Reset State
        m_stop_search = false;
        if (m_search_thread.joinable()) {
            m_search_thread.join();
        }

        SearchParameters effective_params = params;

        if (params.wtime_ms != -1 || params.btime_ms != -1) {
            int time_left = (m_position.us() == Colors::WHITE) ? params.wtime_ms : params.btime_ms;

            if (time_left != -1) {
                int time_to_use = time_left / 50;

                int max_allowable            = std::max(0, time_left - 50);
                effective_params.max_time_ms = std::min(std::max(time_to_use, 5), max_allowable);
            }
        }

        m_search_thread = std::thread(&Engine::search, this, effective_params);
    }

    void search(const SearchParameters& params) {
        m_nodes          = 0;
        m_start_time     = std::chrono::steady_clock::now();
        m_allocated_time = params.max_time_ms;

        std::array<Move, 256> possible_moves{};
        size_t                size = m_position.generateMoves<GenerationTypes::LEGAL>(possible_moves.data());
        if (size == 0) {
            std::cout << "bestmove (none)" << std::endl;
            m_stop_search = true;
            return;
        }

        m_best_move   = possible_moves[0];
        int max_depth = (params.max_depth != -1) ? params.max_depth : 64;

        for (int depth = 1; depth <= max_depth; ++depth) {
            if (m_stop_search) break;

            int alpha = -Evaluation::MATE_SCORE;
            int beta  = Evaluation::MATE_SCORE;

            for (size_t i = 0; i < size; ++i) {
                if (possible_moves[i] == m_best_move) {
                    std::swap(possible_moves[0], possible_moves[i]);
                    break;
                }
            }

            Move current_best_move   = m_best_move;
            int  best_score_at_depth = -Evaluation::MATE_SCORE;

            for (size_t i = 0; i < size; i++) {
                if (m_stop_search) break;

                Move& move = possible_moves[i];
                auto  undo = m_position.makeMove(move);

                int score = -minimax(m_position, depth - 1, -beta, -alpha, 1);

                m_position.unmakeMove(move, undo);

                if (m_stop_search) break;

                if (score > best_score_at_depth) {
                    best_score_at_depth = score;
                    current_best_move   = move;
                }

                alpha = std::max(alpha, best_score_at_depth);
            }

            if (!m_stop_search) {
                m_best_move = current_best_move;
            }
        }

        std::cout << "bestmove " << m_best_move.toString() << std::endl;
        m_stop_search = true;
    }
    int minimax(Position& pos, int depth, int alpha, int beta, int ply) {
        if ((m_nodes++ & 1023) == 0) checkTime();
        if (m_stop_search) return 0;

        if (depth <= 0) {
            return Evaluation::evaluate(pos);
        }

        std::array<Move, 256> moves{};
        size_t                size = pos.generateMoves<GenerationTypes::ALL>(moves.data());

        auto scoreMove = [&](const Move& m) {
            if (pos.at(m.to()) != Pieces::NONE) {
                return 10 * Evaluation::pieceValue(pos.at(m.to()).type()) -
                       Evaluation::pieceValue(pos.at(m.from()).type());
            }
            return 0;
        };
        std::sort(moves.begin(), moves.begin() + size,
                  [&](const Move& a, const Move& b) { return scoreMove(a) > scoreMove(b); });

        int best_score        = -Evaluation::MATE_SCORE;
        int legal_moves_count = 0;

        for (size_t i = 0; i < size; i++) {
            Move& move = moves[i];
            auto  undo = pos.makeMove(move);

            if (pos.isLegal<false>()) {
                legal_moves_count++;

                int score = -minimax(pos, depth - 1, -beta, -alpha, ply + 1);

                pos.unmakeMove(move, undo);

                if (m_stop_search) return 0;

                if (score > best_score) {
                    best_score = score;
                    if (score > alpha) {
                        alpha = score;
                        if (alpha >= beta) break;
                    }
                }
            } else {
                pos.unmakeMove(move, undo);
            }
        }

        if (legal_moves_count == 0) {
            if (pos.isCheck()) return -Evaluation::MATE_SCORE + ply;
            return 0;
        }

        return best_score;
    }

    void checkTime() {
        if (m_allocated_time == -1) return;

        auto now        = std::chrono::steady_clock::now();
        auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_start_time).count();

        if (elapsed_ms >= m_allocated_time) {
            m_stop_search = true;
        }
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

    Move                                  m_best_move = Move();
    std::thread                           m_search_thread;
    std::atomic<bool>                     m_stop_search = false;
    std::chrono::steady_clock::time_point m_start_time;
    int                                   m_allocated_time = -1;
    uint64_t                              m_nodes          = 0;
};