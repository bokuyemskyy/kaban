
#pragma once

#include <array>
#include <cstdint>

#include "strong_value.hpp"

struct Castling : public StrongValue<Castling, uint8_t, 4> {
    using StrongValue::StrongValue;

    constexpr Castling& operator|=(const Castling& other) {
        m_value |= other.m_value;
        return *this;
    }
    constexpr Castling& operator&=(const Castling& other) {
        m_value &= other.m_value;
        return *this;
    }
    [[nodiscard]] constexpr Castling operator|(const Castling& other) const { return Castling(*this) |= other; }
    [[nodiscard]] constexpr Castling operator&(const Castling& other) const { return Castling(*this) &= other; }
    constexpr Castling               operator~() const { return Castling(~m_value & mask()); }

    [[nodiscard]] constexpr bool has(Castling castling) const { return (*this & castling) == castling; }

    constexpr void add(Castling castling) { *this |= castling; }
    constexpr void remove(Castling castling) { *this &= ~castling; }
    constexpr void reset() { *this = Castling(0); }
};

namespace Castlings {
inline constexpr Castling NONE{0};

inline constexpr Castling W_KING_SIDE{1 << 0};
inline constexpr Castling W_QUEEN_SIDE{1 << 1};
inline constexpr Castling B_KING_SIDE{1 << 2};
inline constexpr Castling B_QUEEN_SIDE{1 << 3};

inline constexpr Castling W_SIDE{W_KING_SIDE | W_QUEEN_SIDE};
inline constexpr Castling B_SIDE{B_KING_SIDE | B_QUEEN_SIDE};
inline constexpr Castling ANY{W_SIDE | B_SIDE};

constexpr uint8_t count() noexcept { return 4; }

constexpr std::array<Castling, count()> all() { return {W_KING_SIDE, W_QUEEN_SIDE, B_KING_SIDE, B_QUEEN_SIDE}; }
};  // namespace Castlings
