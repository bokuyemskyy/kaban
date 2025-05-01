#include "movegen.hpp"

#include "bitboard.hpp"
#include "piece.hpp"
#include "square.hpp"

std::array<std::array<Bitboard, Squares::NB>, PieceTypes::NB> pseudoAttacks{};

std::array<std::array<Bitboard, Squares::NB>, Colors::NB>      pawnAttacks{};
std::array<std::array<Bitboard, BISHOP_MAGIC_NB>, Squares::NB> bishopAttacks{};
std::array<std::array<Bitboard, ROOK_MAGIC_NB>, Squares::NB>   rookAttacks{};

void initMovegen() {
    for (Square square = Squares::FIRST; square <= Squares::LAST; ++square) {
        Bitboard knightBB = BITBOARD_ZERO;
        for (const auto& direction : knightDirections) {
            knightBB |= destinationBB(square, direction);
        }
        pseudoAttacks[PieceTypes::KNIGHT][square] = knightBB;

        Bitboard kingBB = BITBOARD_ZERO;
        for (const auto& direction : kingDirections) {
            knightBB |= destinationBB(square, direction);
        }
        pseudoAttacks[PieceTypes::KNIGHT][square] = knightBB;
    }
}