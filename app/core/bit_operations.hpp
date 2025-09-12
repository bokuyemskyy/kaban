#pragma once

#include "bitboard.hpp"
#include "square.hpp"

constexpr Square lsb(const Bitboard& x) { return static_cast<Square>(__builtin_ctzll(x.value())); }
constexpr Square poplsb(Bitboard& x) {
    Square i = lsb(x);
    x &= Bitboard(x.value() - 1);
    return i;
}
constexpr uint8_t popcount(Bitboard x) { return __builtin_popcountll(x.value()); }
