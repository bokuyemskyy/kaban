#pragma once

#include <algorithm>
#include <ranges>
#include <string>
#include <vector>

#include "move.hpp"
#include "move_flag.hpp"
#include "move_list.hpp"
#include "position.hpp"
#include "square.hpp"
#include "undo_info.hpp"

class Engine {
   public:
    void newGame() { m_position.fromFen(); }
    void fromFen(const std::string& fen) { m_position.fromFen(fen); }

    uint64_t perft(int depth) { return m_position.perftRoot(depth); }
    bool     isLegal() { return m_position.isLegal(); }

    [[nodiscard]] auto board() const { return m_position.board(); }
    [[nodiscard]] auto at(Square square) { return m_position.at(square); }
    [[nodiscard]] auto moves() { return MoveList<GenerationType::LEGAL>(m_position); }

    void makeMove(Move move) {
        auto move_list = MoveList<GenerationType::LEGAL>(m_position);
        auto it        = std::ranges::find_if(move_list,
                                              [&](const Move& m) { return m.from() == move.from() && m.to() == move.to(); });
        if (it != move_list.end()) {
            auto undo_info = m_position.makeMove(*it);
            m_history.push(*it, undo_info);
        }
    }
    void makeMove(const std::string& move) {
        if (move.size() != 4 && move.size() != 5) return;

        auto move_list = MoveList<GenerationType::LEGAL>(m_position);

        Move target = Move::fromString(move);

        if (move.size() == 4) {
            auto it = std::ranges::find_if(
                move_list, [&](const Move& m) { return m.from() == target.from() && m.to() == target.to(); });

            if (it != move_list.end()) {
                auto undo_info = m_position.makeMove(*it);
                m_history.push(*it, undo_info);
            }
        } else {
            auto it = std::ranges::find_if(move_list, [&](const Move& m) {
                return m.from() == target.from() && m.to() == target.to() && m.flag() == target.flag();
            });

            if (it != move_list.end()) {
                auto undo_info = m_position.makeMove(*it);
                m_history.push(*it, undo_info);
            }
        }
    }

    void unmakeMove() {
        auto [move, undo_info] = m_history.pop();
        m_position.unmakeMove(move, undo_info);
    }

   private:
    Position m_position;

    struct HistoryEntry {
        Move     move;
        UndoInfo undo_info;
    };

    class History {
       public:
        void push(Move move, UndoInfo undo_info) { m_entries.push_back({move, undo_info}); }

        HistoryEntry pop() {
            if (m_entries.empty()) return {};
            auto last = m_entries.back();
            m_entries.pop_back();
            return last;
        }

       private:
        std::vector<HistoryEntry> m_entries;
    };

    History m_history;
};