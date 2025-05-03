#ifndef MOVEGEN_HPP
#define MOVEGEN_HPP

#include <array>

#include "bitboard.hpp"
#include "magic.hpp"

constexpr uint8_t MAX_MOVES = 256;  // limit of possible number of moves that can be performed from any position

extern std::array<std::array<Bitboard, Squares::NB>, PieceTypes::NB> pseudoAttacks;

extern std::array<std::array<Bitboard, Squares::NB>, Colors::NB>        pawnAttacks;
extern std::array<std::array<Bitboard, ROOK_OCCUPANCY_NB>, Squares::NB> bishopAttacks;
extern std::array<std::array<Bitboard, ROOK_OCCUPANCY_NB>, Squares::NB> rookAttacks;

void initMovegen();

#endif