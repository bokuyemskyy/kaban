#pragma once

#include <array>
#include <cstddef>

#include "move.hpp"
#include "position.hpp"

template <GenerationTypes T>
struct MoveList {
    explicit MoveList(Position& position) : m_last(position.generateMoves<T>(m_data.data())) {}
    const Move* begin() const { return m_data.data(); }
    const Move* end() const { return m_last; }
    size_t      size() const { return m_data.data() - m_last; }
    bool        contains(Move move) const { return std::find(begin(), end(), move) != end(); }

   private:
    static constexpr size_t MAX_MOVES{256};

    std::array<Move, MAX_MOVES> m_data{};
    Move*                       m_last{};
};