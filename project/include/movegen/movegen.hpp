#ifndef MOVEGEN_HPP
#define MOVEGEN_HPP

#include <array>

#include "bitboard.hpp"
#include "magic.hpp"

extern std::array<std::array<Bitboard, Squares::NB>, PieceTypes::NB> pseudoAttacks;

extern std::array<std::array<Bitboard, Squares::NB>, Colors::NB>      pawnAttacks;
extern std::array<std::array<Bitboard, BISHOP_MAGIC_NB>, Squares::NB> bishopAttacks;
extern std::array<std::array<Bitboard, ROOK_MAGIC_NB>, Squares::NB>   rookAttacks;

void initMovegen();

#endif