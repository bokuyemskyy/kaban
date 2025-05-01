#ifndef MAGICS_HPP
#define MAGICS_HPP

#include <array>
#include <cmath>
#include <cstdint>

#include "bitboard.hpp"

using Shift = uint8_t;

struct Magic {
    Bitboard magic;
    Shift    shift;
    Bitboard premask;
};

constexpr int BISHOP_MAGIC_NB = 1 << 9;
constexpr int ROOK_MAGIC_NB   = 1 << 12;

extern std::array<Magic, BISHOP_MAGIC_NB> bishopMagics;
extern std::array<Magic, ROOK_MAGIC_NB>   rookMagics;

void initMagics();

#endif