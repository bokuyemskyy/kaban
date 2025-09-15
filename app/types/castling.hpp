
#pragma once

#include <array>
#include <cstdint>

#include "strong_value.hpp"

class Castling : public StrongValue<Castling, uint8_t> {
   public:
    using StrongValue::StrongValue;

    static constexpr ValueType count() noexcept { return static_cast<ValueType>(4); }
    static constexpr uint8_t   bitlength() { return 4; }
    static constexpr ValueType bitmask() { return static_cast<ValueType>((ValueType(1) << bitlength()) - 1); }

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
    constexpr Castling               operator~() const { return Castling(~m_value & bitmask()); }

    [[nodiscard]] constexpr bool has(Castling castling) const { return (*this & castling) == castling; }

    constexpr void add(Castling castling) { *this |= castling; }
    constexpr void remove(Castling castling) { *this &= ~castling; }
    constexpr void reset() { *this = Castling(0); }

   private:
    // clang-format off
    enum Values : uint8_t {
        NONE        = 0,
        W_KING_SIDE = 1 << 0,  // 0b0001
        W_QUEEN_SIDE= 1 << 1,  // 0b0010
        B_KING_SIDE = 1 << 2,  // 0b0100
        B_QUEEN_SIDE= 1 << 3,  // 0b1000

        W_SIDE      = W_KING_SIDE | W_QUEEN_SIDE,
        B_SIDE      = B_KING_SIDE | B_QUEEN_SIDE,
        KING_SIDE   = W_KING_SIDE | B_KING_SIDE,
        QUEEN_SIDE  = W_QUEEN_SIDE | B_QUEEN_SIDE,
        ANY         = W_SIDE | B_SIDE
    };
    // clang-format on

    friend struct Castlings;
};

struct Castlings {
    static constexpr Castling W_KING_SIDE{Castling::Values::W_KING_SIDE};
    static constexpr Castling W_QUEEN_SIDE{Castling::Values::W_QUEEN_SIDE};
    static constexpr Castling B_KING_SIDE{Castling::Values::B_KING_SIDE};
    static constexpr Castling B_QUEEN_SIDE{Castling::Values::B_QUEEN_SIDE};

    static constexpr Castling W_SIDE{Castling::Values::W_SIDE};
    static constexpr Castling B_SIDE{Castling::Values::B_SIDE};
    static constexpr Castling ANY{Castling::Values::ANY};

    static constexpr std::array<Castling, Castling::count()> all() {
        return {W_KING_SIDE, W_QUEEN_SIDE, B_KING_SIDE, B_QUEEN_SIDE};
    }
};
