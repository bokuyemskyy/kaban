#pragma once

#include <array>
#include <cstdint>

#include "strong_value.hpp"

struct MoveFlag : public StrongValue<MoveFlag, uint8_t, 4> {
    using StrongValue::StrongValue;
};

namespace MoveFlags {
inline constexpr MoveFlag USUAL{0};
inline constexpr MoveFlag PROMOTION_QUEEN{1};
inline constexpr MoveFlag PROMOTION_ROOK{2};
inline constexpr MoveFlag PROMOTION_BISHOP{3};
inline constexpr MoveFlag PROMOTION_KNIGHT{4};
inline constexpr MoveFlag CASTLING_KING{5};
inline constexpr MoveFlag CASTLING_QUEEN{6};
inline constexpr MoveFlag EN_PASSANT{7};
inline constexpr MoveFlag PAWN_DOUBLE_PUSH{8};

constexpr uint8_t count() noexcept { return 9; }

constexpr std::array<MoveFlag, count()> all() {
    return {USUAL,         PROMOTION_QUEEN, PROMOTION_ROOK, PROMOTION_BISHOP, PROMOTION_KNIGHT,
            CASTLING_KING, CASTLING_QUEEN,  EN_PASSANT,     PAWN_DOUBLE_PUSH};
}
};  // namespace MoveFlags
