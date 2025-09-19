#pragma once

#include "move.hpp"
#include "move_flag.hpp"
#include "square.hpp"
#include "strong_value.hpp"

struct Move : public StrongValue<Move, uint16_t> {
    using StrongValue::StrongValue;
    constexpr Move(Square from, Square to, MoveFlag move_flag = MoveFlags::USUAL)
        : StrongValue((move_flag.value() << MOVEFLAG_SHIFT) | (to.value() << TO_SHIFT) | from.value()) {}

    [[nodiscard]] constexpr bool hasValue() const { return m_value != 0; }

    [[nodiscard]] constexpr Square from() const { return Square(m_value & Square::mask()); }

    [[nodiscard]] constexpr Square to() const { return Square((m_value >> TO_SHIFT) & Square::mask()); }

    [[nodiscard]] constexpr MoveFlag flag() const { return MoveFlag((m_value >> MOVEFLAG_SHIFT) & MoveFlag::mask()); }

    static constexpr Move fromString(const std::string& str) {
        MoveFlag promotion = MoveFlags::USUAL;
        if (str.size() == 5) {
            switch (str[5]) {
                case 'q':
                    promotion = MoveFlags::PROMOTION_QUEEN;
                    break;
                case 'r':
                    promotion = MoveFlags::PROMOTION_ROOK;
                    break;
                case 'b':
                    promotion = MoveFlags::PROMOTION_BISHOP;
                    break;
                case 'n':
                    promotion = MoveFlags::PROMOTION_KNIGHT;
                    break;
            }
        }
        return Move(Square::fromString(str.substr(0, 2)), Square::fromString(str.substr(2, 2)), promotion);
    }

    [[nodiscard]] constexpr std::string toString() const {
        std::string promotion = "";

        if (flag() == MoveFlags::PROMOTION_QUEEN) {
            promotion = "q";
        } else if (flag() == MoveFlags::PROMOTION_ROOK) {
            promotion = "r";
        } else if (flag() == MoveFlags::PROMOTION_BISHOP) {
            promotion = "b";
        } else if (flag() == MoveFlags::PROMOTION_KNIGHT) {
            promotion = "n";
        }

        return from().toString() + to().toString() + promotion;
    }

   private:
    static constexpr int FROM_SHIFT     = 0;
    static constexpr int TO_SHIFT       = Square::width();
    static constexpr int MOVEFLAG_SHIFT = 2 * Square::width();
};
