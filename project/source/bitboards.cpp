#include "bitboards.hpp"

#include <array>
#include <cstdint>
#include <cstdlib>

#include "piece.hpp"
#include "square.hpp"

std::array<Bitboard, Ranks::NB> rankBB{};
std::array<Bitboard, Files::NB> fileBB{};

std::array<std::array<uint8_t, Squares::NB>, Squares::NB> squareDistances{};

std::array<std::array<Bitboard, Squares::NB>, Colors::NB>     pawnAttacks{};
std::array<std::array<Bitboard, Squares::NB>, PieceTypes::NB> pseudoAttacks{};

void precompute() {
    const Bitboard RANK_A = 0xFFULL;
    for (Rank rank = 0; rank <= Ranks::LAST; ++rank) rankBB[rank] = RANK_A << (rank * Squares::NB);

    constexpr Bitboard FILE_A = 0x0101010101010101ULL;
    for (File file = 0; file <= Files::LAST; ++file) fileBB[file] = FILE_A << file;

    for (Square from = Squares::FIRST; from <= Squares::LAST; ++from) {
        for (Square to = Squares::FIRST; to <= Squares::LAST; ++to) {
            squareDistances[from][to] = std::max(abs(getFile(from) - getFile(to)), abs(getRank(from) - getRank(to)));
        }
    }

    for (Square square = 0; square <= Squares::LAST; ++square) {
        for (auto direction :
             {Directions::EAST, Directions::NORTH, Directions::WEST, Directions::SOUTH, Directions::NORTH_EAST,
              Directions::NORTH_WEST, Directions::SOUTH_EAST, Directions::SOUTH_WEST}) {
            Bitboard destination = destinationBB(square, direction);
            if (destination != BITBOARD_ZERO) {
                pseudoAttacks[PieceTypes::KING][square] |= destination;
            }
        }
    }

    for (Square square = 0; square <= Squares::LAST; ++square) {
        for (auto direction :
             {Directions::EAST, Directions::NORTH, Directions::WEST, Directions::SOUTH, Directions::NORTH_EAST,
              Directions::NORTH_WEST, Directions::SOUTH_EAST, Directions::SOUTH_WEST}) {
            Bitboard destination = destinationBB(square, direction);
            if (destination != BITBOARD_ZERO) {
                pseudoAttacks[PieceTypes::KING][square] |= destination;
            }
        }
    }
}

Bitboard destinationBB(Square square, Direction direction) {
    Square destination = square + direction;
    return (isSquareInBounds(destination) && squareDistances[square][destination] <= 2) ? squareBB(destination)
                                                                                        : Bitboard{};
}
