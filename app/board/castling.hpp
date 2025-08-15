#pragma once

#include <array>
#include <cstdint>

#include "strong.hpp"

class Castling : public Strong::Value<Castling, uint8_t>,
                 public Strong::Enumerator<Castling, uint8_t, 4>,
                 public Strong::Field<Castling, uint8_t, 4> {
   public:
    using Value::Value;

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
    constexpr void reset() { set(Values::ANY); }

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
    // clang-format off
    #define C(name) \
        static constexpr Castling name { Castling::Values::name }

        C(W_KING_SIDE); C(W_QUEEN_SIDE); C(B_KING_SIDE); C(B_QUEEN_SIDE);
        C(W_SIDE); C(B_SIDE); C(ANY);
    #undef C

    static constexpr std::array<Castling, Castling::number()> all() {
        return {W_KING_SIDE, W_QUEEN_SIDE, B_KING_SIDE, B_QUEEN_SIDE};
    }
    // clang-format on
};
