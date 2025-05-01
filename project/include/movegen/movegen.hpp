#ifndef MOVEGEN_HPP
#define MOVEGEN_HPP

#include <array>

#include "magic.hpp"
#include "piece.hpp"
#include "square.hpp"
#include "types.hpp"

extern std::array<Bitboard, Ranks::NB> rankBB;
extern std::array<Bitboard, Files::NB> fileBB;

extern std::array<std::array<uint8_t, Squares::NB>, Squares::NB> squareDistance;

const int PAWN_DIRECTIONS_NB   = 2;
const int KNIGHT_DIRECTIONS_NB = 8;
const int KING_DIRECTIONS_NB   = 8;

inline std::array<Direction, KNIGHT_DIRECTIONS_NB> knightDirections = {
    Directions::NORTH + Directions::NORTH + Directions::EAST, Directions::NORTH + Directions::NORTH + Directions::WEST,
    Directions::NORTH + Directions::EAST + Directions::EAST,  Directions::NORTH + Directions::WEST + Directions::WEST,
    Directions::SOUTH + Directions::SOUTH + Directions::EAST, Directions::SOUTH + Directions::SOUTH + Directions::WEST,
    Directions::SOUTH + Directions::EAST + Directions::EAST,  Directions::SOUTH + Directions::WEST + Directions::WEST};

inline std::array<Direction, KING_DIRECTIONS_NB> kingDirections = {
    Directions::EAST,       Directions::NORTH,      Directions::WEST,       Directions::SOUTH,
    Directions::NORTH_EAST, Directions::NORTH_WEST, Directions::SOUTH_EAST, Directions::SOUTH_WEST};

extern std::array<std::array<Bitboard, Squares::NB>, PieceTypes::NB> pseudoAttacks;

extern std::array<std::array<Bitboard, Squares::NB>, Colors::NB>      pawnAttacks;
extern std::array<std::array<Bitboard, BISHOP_MAGIC_NB>, Squares::NB> bishopAttacks;
extern std::array<std::array<Bitboard, ROOK_MAGIC_NB>, Squares::NB>   rookAttacks;

Bitboard destinationBB(Square square, Direction direction);

void initMovegen();

#endif