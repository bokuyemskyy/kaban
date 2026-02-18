#pragma once

#include <algorithm>
#include <chrono>
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
    explicit Engine(bool run_search_on_change = false, int search_max_time_ms = 5000) {
        Magics::get();

        if (run_search_on_change) {
            m_search_max_time_ms = search_max_time_ms;
        }
        trySearchOnChange();
    }
    void newGame() { m_position.fromFen(); }

    void        fromFen(const std::string& fen) { m_position.fromFen(fen); }
    std::string toFen() { return m_position.toFen(); }

    void go(const SearchParameters& parameters) {
        m_stop_search = false;
        if (m_search_thread.joinable()) {
            m_search_thread.join();
        }

        SearchParameters effective_parameters = parameters;

        if (parameters.wtime_ms != -1 || parameters.btime_ms != -1) {
            int time_left = (m_position.us() == Colors::WHITE) ? parameters.wtime_ms : parameters.btime_ms;

            if (time_left != -1) {
                int time_to_use = time_left / 50;

                int max_allowable                = std::max(0, time_left - 50);
                effective_parameters.max_time_ms = std::min(std::max(time_to_use, 5), max_allowable);
            }
        }

        m_search_thread = std::thread(&Engine::search, this, m_position, effective_parameters);
    }

    void search(Position position, const SearchParameters& parameters) {
        m_nodes          = 0;
        m_start_time     = std::chrono::steady_clock::now();
        m_allocated_time = parameters.max_time_ms;

        std::array<Move, 256> possible_moves{};
        size_t                size = position.generateMoves<GenerationTypes::LEGAL>(possible_moves.data());
        if (size == 0) {
            std::cout << "bestmove (none)" << std::endl;
            m_stop_search = true;
            return;
        }

        m_best_move   = possible_moves[0];
        int max_depth = (parameters.max_depth != -1) ? parameters.max_depth : 64;

        for (int depth = 1; depth <= max_depth; ++depth) {
            if (m_stop_search) break;

            m_current_depth = depth;
            int alpha       = -Evaluation::MATE_SCORE;
            int beta        = Evaluation::MATE_SCORE;

            auto it = std::find_if(possible_moves.begin(), possible_moves.end(),
                                   [this](const Move& move) { return move == this->m_best_move; });

            if (it != possible_moves.end()) {
                std::swap(possible_moves[0], *it);
            }

            Move current_best_move   = m_best_move;
            int  best_score_at_depth = -Evaluation::MATE_SCORE;

            for (size_t i = 0; i < size; i++) {
                if (m_stop_search) break;

                Move const& move = possible_moves[i];
                auto        undo = position.makeMove(move);

                int score = -minimax(position, depth - 1, -beta, -alpha, 1);

                position.unmakeMove(move, undo);

                if (m_stop_search) break;

                if (score > best_score_at_depth) {
                    best_score_at_depth = score;
                    current_best_move   = move;
                }

                alpha = std::max(alpha, best_score_at_depth);
            }

            if (!m_stop_search) {
                m_best_move         = current_best_move;
                m_current_best_move = current_best_move;
                m_current_eval      = best_score_at_depth;
            }
        }

        std::cout << "bestmove " << m_best_move.toString() << std::endl;
        m_stop_search = true;
    }
    int minimax(Position& position, int depth, int alpha, int beta, int ply) {
        if ((m_nodes++ & 1023) == 0) checkTime();
        if (m_stop_search) return 0;

        if (depth <= 0) {
            return Evaluation::evaluate(position);
        }

        std::array<Move, 256> moves_{};
        size_t                size = position.generateMoves<GenerationTypes::ALL>(moves_.data());

        auto scoreMove = [&](const Move& m) {
            if (position.at(m.to()) != Pieces::NONE) {
                return 10 * Evaluation::pieceValue(position.at(m.to()).type()) -
                       Evaluation::pieceValue(position.at(m.from()).type());
            }
            return 0;
        };
        std::sort(moves_.begin(), moves_.begin() + size,
                  [&](const Move& a, const Move& b) { return scoreMove(a) > scoreMove(b); });

        int best_score        = -Evaluation::MATE_SCORE;
        int legal_moves_count = 0;

        for (size_t i = 0; i < size; i++) {
            Move const& move = moves_[i];
            auto        undo = position.makeMove(move);

            if (position.isLegal<false>()) {
                legal_moves_count++;

                int score = -minimax(position, depth - 1, -beta, -alpha, ply + 1);

                position.unmakeMove(move, undo);

                if (m_stop_search) return 0;

                if (score > best_score) {
                    best_score = score;
                    if (score > alpha) {
                        alpha = score;
                        if (alpha >= beta) break;
                    }
                }
            } else {
                position.unmakeMove(move, undo);
            }
        }

        if (legal_moves_count == 0) {
            if (position.isCheck()) return -Evaluation::MATE_SCORE + ply;
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

    [[nodiscard]] auto board() const { return m_position.board(); }
    [[nodiscard]] auto at(Square square) { return m_position.at(square); }

    [[nodiscard]] std::vector<Move>& moves() {
        if (!m_moves_dirty) return m_moves_cache;

        std::array<Move, 256> move_list{};
        size_t                size = m_position.generateMoves<GenerationTypes::LEGAL>(move_list.data());

        m_moves_cache.assign(move_list.begin(), move_list.begin() + size);
        m_moves_dirty = false;
        return m_moves_cache;
    }

    void makeMove(Move move) {
        m_moves_dirty = true;

        std::array<Move, 256> move_list{};
        size_t                size = m_position.generateMoves<GenerationTypes::LEGAL>(move_list.data());
        auto                  it   = std::ranges::find_if(move_list.begin(), move_list.begin() + size,
                                                          [&](const Move& m) { return m.from() == move.from() && m.to() == move.to(); });

        if (it == move_list.begin() + size) throw std::runtime_error("Invalid move");

        auto undo_info = m_position.makeMove(*it);
        m_history.push(*it, undo_info);

        trySearchOnChange();
    }
    void trySearchOnChange() {
        if (m_search_max_time_ms > 0) {
            stop();
            go(SearchParameters{.max_time_ms = m_search_max_time_ms});
        }
    }
    void makeMove(const std::string& move) {
        m_moves_dirty = true;

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

        trySearchOnChange();
    }

    void unmakeMove() {
        m_moves_dirty = true;

        auto [move, undo_info] = m_history.pop();
        m_position.unmakeMove(move, undo_info);

        trySearchOnChange();
    }

    [[nodiscard]] Move  getCurrentBestMove() const { return m_current_best_move; }
    [[nodiscard]] int   getCurrentDepth() const { return m_current_depth; }
    [[nodiscard]] int   getCurrentEval() const { return m_current_eval; }
    [[nodiscard]] Color getSideToMove() const { return m_position.us(); }
    [[nodiscard]] bool  isGameOver() {
        auto _moves = moves();
        return _moves.empty();
    }
    [[nodiscard]] bool              isCheck() { return m_position.isCheck(); }
    [[nodiscard]] std::vector<Move> getMoveHistory() const { return m_history.getMoveHistory(); }

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

    Move m_best_move{};
    Move m_current_best_move{};
    int  m_current_depth{};
    int  m_current_eval{};

    std::thread                           m_search_thread{};
    std::atomic<bool>                     m_stop_search{};
    std::chrono::steady_clock::time_point m_start_time{};

    int m_search_max_time_ms{-1};

    bool              m_moves_dirty{true};
    std::vector<Move> m_moves_cache{};

    int      m_allocated_time{-1};
    uint64_t m_nodes{};
};