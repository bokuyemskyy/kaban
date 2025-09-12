#pragma once

#include <ranges>
#include <string>

#include "move.hpp"
#include "position.hpp"
#include "square.hpp"

class Engine {
   public:
    void set_position(const std::string& fen) { m_position.set_from_fen(fen); }

    void do_move(Move move) { m_position.do_move(move); }

    uint64_t perft(int depth) { return 0; /*m_position.perft(depth);*/ }

    [[nodiscard]] auto board() const { return m_position.board(); }
    [[nodiscard]] auto at(Square square) { return m_position.at(square); }
    [[nodiscard]] auto moves() const { return m_position.generate_moves<GenerationType::LEGAL>(); }
    [[nodiscard]] auto moves(Square square) const {
        std::vector<Move> filtered;
        auto              all = m_position.generate_moves<GenerationType::LEGAL>();
        std::copy_if(all.begin(), all.end(), std::back_inserter(filtered),
                     [square](const Move& m) { return m.from() == square; });
        return filtered;
    }

   private:
    Position m_position;
};