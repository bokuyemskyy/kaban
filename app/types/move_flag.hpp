#pragma once

#include <array>
#include <cstdint>

#include "strong_value.hpp"

class MoveFlag : public StrongValue<MoveFlag, uint8_t> {
   public:
    using StrongValue::StrongValue;

    static constexpr ValueType count() noexcept { return static_cast<ValueType>(9); }
    static constexpr uint8_t   bitlength() { return 4; }
    static constexpr ValueType bitmask() { return static_cast<ValueType>((ValueType(1) << bitlength()) - 1); }

   private:
    // clang-format off
    enum Values : uint8_t {
        USUAL            = 0,
        PROMOTION_QUEEN  = 1,
        PROMOTION_ROOK   = 2,
        PROMOTION_BISHOP = 3,
        PROMOTION_KNIGHT = 4,
        CASTLING_KING    = 5,
        CASTLING_QUEEN   = 6,
        EN_PASSANT       = 7,
        PAWN_DOUBLE_PUSH = 8
    };
    // clang-format on

    friend struct MoveFlags;
};

struct MoveFlags {
    static constexpr MoveFlag USUAL{MoveFlag::Values::USUAL};
    static constexpr MoveFlag PROMOTION_QUEEN{MoveFlag::Values::PROMOTION_QUEEN};
    static constexpr MoveFlag PROMOTION_ROOK{MoveFlag::Values::PROMOTION_ROOK};
    static constexpr MoveFlag PROMOTION_BISHOP{MoveFlag::Values::PROMOTION_BISHOP};
    static constexpr MoveFlag PROMOTION_KNIGHT{MoveFlag::Values::PROMOTION_KNIGHT};
    static constexpr MoveFlag CASTLING_KING{MoveFlag::Values::CASTLING_KING};
    static constexpr MoveFlag CASTLING_QUEEN{MoveFlag::Values::CASTLING_QUEEN};
    static constexpr MoveFlag EN_PASSANT{MoveFlag::Values::EN_PASSANT};
    static constexpr MoveFlag PAWN_DOUBLE_PUSH{MoveFlag::Values::PAWN_DOUBLE_PUSH};

    static constexpr std::array<MoveFlag, MoveFlag::count()> all() {
        return {USUAL,         PROMOTION_QUEEN, PROMOTION_ROOK, PROMOTION_BISHOP, PROMOTION_KNIGHT,
                CASTLING_KING, CASTLING_QUEEN,  EN_PASSANT,     PAWN_DOUBLE_PUSH};
    }
};
