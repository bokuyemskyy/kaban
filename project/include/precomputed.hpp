#ifndef PRECOMPUTED_HPP
#define PRECOMPUTED_HPP

#include <array>

#include "piece.hpp"
#include "square.hpp"
#include "types.hpp"

extern std::array<std::array<Bitboard, SQUARE_NB>, COLOR_NB> pawnAttacks;
extern std::array<std::array<Bitboard, SQUARE_NB>, PIECETYPE_NB> pseudoAttacks;

void precomputeAttacks();

#endif