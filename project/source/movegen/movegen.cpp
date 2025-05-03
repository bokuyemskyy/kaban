#include "movegen.hpp"

#include "bitboard.hpp"
#include "magic.hpp"
#include "piece.hpp"
#include "square.hpp"

std::array<std::array<Bitboard, Squares::NB>, PieceTypes::NB> pseudoAttacks{};

std::array<std::array<Bitboard, Squares::NB>, Colors::NB>        pawnAttacks{};
std::array<std::array<Bitboard, ROOK_OCCUPANCY_NB>, Squares::NB> bishopAttacks{};
std::array<std::array<Bitboard, ROOK_OCCUPANCY_NB>, Squares::NB> rookAttacks{};

Bitboard getRookAttacks(Square square, Bitboard occupancy) {
    Bitboard attackBB = BITBOARD_ZERO;
    for (const auto& direction : rookDirections) {
        for (uint8_t i = 1;; ++i) {
            Bitboard destination = destinationBB(square + (direction * (i - 1)), direction);
            if (destination == BITBOARD_ZERO) break;

            attackBB |= destination;

            if (occupancy & destination) break;
        }
    }
    return attackBB;
}

void initMovegen() {
    initMagics();

    for (Square square = Squares::FIRST; square <= Squares::LAST; ++square) {
        Bitboard pawnBB = BITBOARD_ZERO;
        for (const auto& direction : pawnDirections[Colors::WHITE]) {
            pawnBB |= destinationBB(square, direction);
        }
        pawnAttacks[Colors::WHITE][square] = pawnBB;

        pawnBB = BITBOARD_ZERO;
        for (const auto& direction : pawnDirections[Colors::BLACK]) {
            pawnBB |= destinationBB(square, direction);
        }
        pawnAttacks[Colors::BLACK][square] = pawnBB;

        Bitboard knightBB = BITBOARD_ZERO;
        for (const auto& direction : knightDirections) {
            knightBB |= destinationBB(square, direction);
        }
        pseudoAttacks[PieceTypes::KNIGHT][square] = knightBB;

        for (const auto& occupancy : rookOccupancies[square]) {
            rookAttacks[square][magicMap(square, occupancy)] = getRookAttacks(square, occupancy);
        }

        Bitboard kingBB = BITBOARD_ZERO;
        for (const auto& direction : kingDirections) {
            kingBB |= destinationBB(square, direction);
        }
        pseudoAttacks[PieceTypes::KING][square] = kingBB;
    }
}