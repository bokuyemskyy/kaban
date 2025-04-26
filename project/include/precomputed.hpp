#ifndef PRECOMPUTED_HPP
#define PRECOMPUTED_HPP

#include <array>

#include "piece.hpp"
#include "square.hpp"

extern std::array<std::array<uint8_t, Squares::NB>, Squares::NB> squareDistance;

extern std::array<std::array<Bitboard, Squares::NB>, Colors::NB>     pawnAttacks;
extern std::array<std::array<Bitboard, Squares::NB>, PieceTypes::NB> pseudoAttacks;

constexpr Bitboard getDestination(Square square, Direction direction);

void precomputeSquareDistance();
void precomputeAttacks();
void precomputeAll();

#endif