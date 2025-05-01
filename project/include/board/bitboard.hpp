#ifndef PRECOMPUTED_HPP
#define PRECOMPUTED_HPP

#include <array>
#include <cstdint>

#include "direction.hpp"
#include "square.hpp"

using Bitboard = uint64_t;

constexpr Bitboard BITBOARD_ZERO = 0ULL;

extern std::array<Bitboard, Ranks::NB> rankBB;
extern std::array<Bitboard, Files::NB> fileBB;

void initBitboards();

constexpr Bitboard squareBB(Square square) {
    assert(isValid(square));
    return (1ULL << square);
}
constexpr Bitboard destinationBB(Square square, Direction direction) {
    Square destination = square + direction;
    return (isValid(destination) && squareDistance[square][destination] <= 2) ? squareBB(destination) : BITBOARD_ZERO;
};

void initBitboards();

#endif