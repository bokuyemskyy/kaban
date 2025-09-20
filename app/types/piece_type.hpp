#pragma once

#include <array>
#include <cassert>
#include <cctype>
#include <cstdint>

#include "strong_value.hpp"

struct PieceType : public StrongValue<PieceType, uint8_t, 3> {
    using StrongValue::StrongValue;

    static constexpr PieceType fromChar(char c) {
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        for (uint8_t i = 0; i < 6; ++i)
            if (chars[i] == c) return PieceType(i);
        return PieceType();
    }
    [[nodiscard]] constexpr char toChar() const { return chars[m_value]; }

   private:
    static constexpr std::array<char, 6> chars = {'p', 'n', 'b', 'r', 'q', 'k'};
};

namespace PieceTypes {
inline constexpr PieceType PAWN{0};
inline constexpr PieceType KNIGHT{1};
inline constexpr PieceType BISHOP{2};
inline constexpr PieceType ROOK{3};
inline constexpr PieceType QUEEN{4};
inline constexpr PieceType KING{5};

constexpr uint8_t count() noexcept { return 6; }

constexpr std::array<PieceType, count()> all() { return {PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING}; }
};  // namespace PieceTypes