#pragma once

#include <algorithm>
#include <ranges>
#include <string>
#include <vector>

#include "move.hpp"
#include "position.hpp"
#include "square.hpp"
#include "undo_info.hpp"

class Engine {
   public:
    void newGame() { m_position.fromFen(); }
    void setPosition(const std::string& fen) { m_position.fromFen(fen); }

    uint64_t perft(int depth) { return 0; /*m_position.perft(depth);*/ }

    [[nodiscard]] auto board() const { return m_position.board(); }
    [[nodiscard]] auto at(Square square) { return m_position.at(square); }
    [[nodiscard]] auto moves() const { return m_position.generateMoves<GenerationType::LEGAL>(); }
    [[nodiscard]] auto moves(Square square) const { return m_position.generateMoves<GenerationType::LEGAL>(square); }

    void makeMove(Move move) {
        auto moves = m_position.generateMoves<GenerationType::LEGAL>();
        auto it =
            std::ranges::find_if(moves, [&](const Move& m) { return m.from() == move.from() && m.to() == move.to(); });
        if (it != moves.end()) {
            auto undo_info = m_position.makeMove(*it);
            m_history.push(*it, undo_info);
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