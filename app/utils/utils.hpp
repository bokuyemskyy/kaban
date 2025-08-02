#pragma once

#include <cmath>

#include "bitboard.hpp"

constexpr float eucledianDistance(const std::pair<float, float>& a, const std::pair<float, float>& b) {
    return std::sqrt(((a.first - b.first) * (a.first - b.first)) + ((a.second - b.second) * (a.second - b.second)));
}

constexpr Square lsb(const Bitboard& x) { return static_cast<Square>(__builtin_ctzll(x)); }
constexpr Square popLSB(Bitboard& x) {
    Square i = lsb(x);
    x &= x - 1;
    return i;
}
constexpr uint8_t popCount(Bitboard x) { return __builtin_popcountll(x); }
