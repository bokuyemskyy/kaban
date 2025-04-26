#ifndef PRECOMPUTED_HPP
#define PRECOMPUTED_HPP

#include <array>

#include "piece.hpp"
#include "square.hpp"

extern std::array<std::array<uint8_t, Squares::NB>, Squares::NB> squareDistances;

extern std::array<std::array<Bitboard, Squares::NB>, Colors::NB>     pawnAttacks;
extern std::array<std::array<Bitboard, Squares::NB>, PieceTypes::NB> pseudoAttacks;

Bitboard destinationBB(Square square, Direction direction);

void precompute();

#endif