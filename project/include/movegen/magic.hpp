#ifndef MAGICS_HPP
#define MAGICS_HPP

#include <array>
#include <cmath>
#include <cstdint>
#include <vector>

#include "bitboard.hpp"

using Shift = uint8_t;

struct Magic {
    Bitboard magic;
    Shift    shift;
    Bitboard premask;
};

constexpr int BISHOP_OCCUPANCY_NB = 1 << 9;
constexpr int ROOK_OCCUPANCY_NB   = 1 << 12;

extern std::array<Magic, Squares::NB> bishopMagics;
extern std::array<Magic, Squares::NB> rookMagics;

extern std::array<std::vector<Bitboard>, Squares::NB> bishopOccupancies;
extern std::array<std::vector<Bitboard>, Squares::NB> rookOccupancies;

Bitboard getRookPremask();
Bitboard getBishopPremask();

void initOccupancies();
void initMagics();

constexpr int magicMap(Square square, Bitboard occupancy) {
    return static_cast<int>(((occupancy & rookMagics[square].premask) * rookMagics[square].magic) >>
                            rookMagics[square].shift);
}

#endif