#pragma once

#include "move.hpp"
#include "move_flag.hpp"
#include "square.hpp"
#include "strong_value.hpp"

class Move : StrongValue<Move, uint16_t> {
   public:
    using StrongValue::StrongValue;
    constexpr Move(Square from, Square to, MoveFlag move_flag = MoveFlags::USUAL)
        : StrongValue((move_flag.value() << MOVEFLAG_SHIFT) | (to.value() << TO_SHIFT) | from.value()) {}

    [[nodiscard]] constexpr bool hasValue() const { return m_value != 0; }

    [[nodiscard]] constexpr Square from() const { return Square(m_value & Square::bitmask()); }

    [[nodiscard]] constexpr Square to() const { return Square((m_value >> TO_SHIFT) & Square::bitmask()); }

    [[nodiscard]] constexpr MoveFlag flag() const {
        return MoveFlag((m_value >> MOVEFLAG_SHIFT) & MoveFlag::bitmask());
    }

   private:
    static constexpr int FROM_SHIFT     = 0;
    static constexpr int TO_SHIFT       = Square::bitlength();
    static constexpr int MOVEFLAG_SHIFT = 2 * Square::bitlength();
};
