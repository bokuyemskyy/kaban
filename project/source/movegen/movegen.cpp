#include "movegen.hpp"

#include "bitboards.hpp"
#include "piece.hpp"
#include "square.hpp"
#include "types.hpp"

std::array<Bitboard, Ranks::NB>                           rankBB{};
std::array<Bitboard, Files::NB>                           fileBB{};
std::array<std::array<uint8_t, Squares::NB>, Squares::NB> squareDistance{};

std::array<std::array<Bitboard, Squares::NB>, PieceTypes::NB> pseudoAttacks{};

std::array<std::array<Bitboard, Squares::NB>, Colors::NB>      pawnAttacks{};
std::array<std::array<Bitboard, BISHOP_MAGIC_NB>, Squares::NB> bishopAttacks{};
std::array<std::array<Bitboard, ROOK_MAGIC_NB>, Squares::NB>   rookAttacks{};

Bitboard destinationBB(Square square, Direction direction) {
    Square destination = square + direction;
    return (isSquareInBounds(destination) && squareDistance[square][destination] <= 2) ? squareBB(destination)
                                                                                       : Bitboard{};
}

void initHelpers() {
    constexpr Bitboard RANK_A = 0xFFULL;
    for (Rank rank = 0; rank <= Ranks::LAST; ++rank) rankBB[rank] = RANK_A << (rank * Squares::NB);

    constexpr Bitboard FILE_A = 0x0101010101010101ULL;
    for (File file = 0; file <= Files::LAST; ++file) fileBB[file] = FILE_A << file;

    for (Square from = Squares::FIRST; from <= Squares::LAST; ++from) {
        for (Square to = Squares::FIRST; to <= Squares::LAST; ++to) {
            squareDistance[from][to] = std::max(abs(getFile(from) - getFile(to)), abs(getRank(from) - getRank(to)));
        }
    }
}

void initMovegen() {
    initHelpers();
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