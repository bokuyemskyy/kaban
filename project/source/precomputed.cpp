#include "precomputed.hpp"

#include <array>
#include <cstdlib>

#include "piece.hpp"
#include "square.hpp"

std::array<std::array<uint8_t, Squares::NB>, Squares::NB> squareDistance{};

std::array<std::array<Bitboard, Squares::NB>, Colors::NB>     pawnAttacks{};
std::array<std::array<Bitboard, Squares::NB>, PieceTypes::NB> pseudoAttacks{};

constexpr Bitboard getDestination(Square square, Direction direction) {
    Square destination = square + direction;
    return (isSquareInBounds(destination) && squareDistance[square][destination] <= 2) ? squareBB(destination)
                                                                                       : BITBOARD_ZERO;
}

void precomputeSquareDistance() {
    for (Square square1 = Squares::FIRST; square1 <= Squares::LAST; square1++) {
        for (Square square2 = Squares::FIRST; square2 <= Squares::LAST; square2++) {
            squareDistance[square1][square2] =
                std::max(abs(getFile(square1) - getFile(square2)), abs(getRank(square1) - getRank(square2)));
        }
    }
}
void precomputeAttacks() {
    for (Square square = Squares::FIRST; square <= Squares::LAST; square++) {
        for (auto direction :
             {Directions::EAST, Directions::NORTH, Directions::WEST, Directions::SOUTH, Directions::NORTH_EAST,
              Directions::NORTH_WEST, Directions::SOUTH_EAST, Directions::SOUTH_WEST}) {
            Bitboard destination = getDestination(square, direction);
            if (destination != BITBOARD_ZERO) {
                pseudoAttacks[PieceTypes::KING][square] |= destination;
            }
        }
    }
}
void precomputeAll() {
    precomputeSquareDistance();
    precomputeAttacks();
}