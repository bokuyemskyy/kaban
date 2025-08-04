#pragma once

#include <cstdint>

#include "piece.hpp"

struct Castling : Metadata<Castling> {
    enum : uint8_t {
        W_KING_SIDE  = 0b0001,
        W_QUEEN_SIDE = 0b0010,
        B_KING_SIDE  = 0b0100,
        B_QUEEN_SIDE = 0b1000,

        KING_SIDE  = W_KING_SIDE | B_KING_SIDE,    // 0b0101
        QUEEN_SIDE = W_QUEEN_SIDE | B_QUEEN_SIDE,  // 0b1010
        W_SIDE     = W_KING_SIDE | W_QUEEN_SIDE,   // 0b0011
        B_SIDE     = B_KING_SIDE | B_QUEEN_SIDE,   // 0b1100
        ANY        = W_SIDE | B_SIDE,              // 0b1111

        NONE = 0b0000,

        MASK = 0b1111,
        SIZE = 4,
        NB   = 4
    };

    constexpr Castling(uint8_t value) : m_value(value) {}

    [[nodiscard]] static constexpr uint8_t none() { return NONE; }

    constexpr operator uint8_t() const { return m_value; }

    [[nodiscard]] constexpr uint8_t value() const { return m_value; }

   private:
    uint8_t m_value;
};