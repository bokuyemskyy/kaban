#pragma once

#include "move.hpp"
#include "move_type.hpp"
#include "promotion.hpp"
#include "square.hpp"
#include "strong.hpp"

class Move : Strong::Value<Move, uint16_t> {
   public:
    using Value::Value;
    constexpr Move(Square from, Square to, MoveType moveType = MoveTypes::USUAL,
                   Promotion promotion = Promotions::TO_QUEEN)
        : Value((promotion.value() << PROMOTION_SHIFT) | (moveType.value() << MOVETYPE_SHIFT) |
                (to.value() << TO_SHIFT) | from.value()) {}

    [[nodiscard]] constexpr Square from() const { return Square(m_value & Square::mask()); }

    [[nodiscard]] constexpr Square to() const { return Square((m_value >> TO_SHIFT) & Square::mask()); }

    [[nodiscard]] constexpr MoveType moveType() const {
        return MoveType((m_value >> MOVETYPE_SHIFT) & MoveType::mask());
    }

    [[nodiscard]] constexpr Promotion promotion() const {
        return Promotion((m_value >> PROMOTION_SHIFT) & Promotion::mask());
    }

    [[nodiscard]] constexpr bool hasValue() const { return m_value != 0; }

   private:
    static constexpr int FROM_SHIFT      = 0;
    static constexpr int TO_SHIFT        = Square::size();
    static constexpr int MOVETYPE_SHIFT  = 2 * Square::size();
    static constexpr int PROMOTION_SHIFT = (2 * Square::size()) + MoveType::size();
};
