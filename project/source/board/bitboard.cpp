#include "bitboard.hpp"

#include "square.hpp"

std::array<Bitboard, Ranks::NB> rankBB{};
std::array<Bitboard, Files::NB> fileBB{};

std::array<Bitboard, Squares::NB> diagBB{};
std::array<Bitboard, Squares::NB> antiDiagBB{};

void initBitboards() {
    constexpr Bitboard RANK_A = 0xFFULL;
    for (Rank rank = 0; rank <= Ranks::LAST; ++rank) rankBB[rank] = (RANK_A << (rank * Files::NB));

    constexpr Bitboard FILE_A = 0x0101010101010101ULL;
    for (File file = 0; file <= Files::LAST; ++file) fileBB[file] = FILE_A << file;

    for (Square square = Squares::FIRST; square <= Squares::LAST; ++square) {
        Bitboard diag = BITBOARD_ZERO, antiDiag = BITBOARD_ZERO;

        File squareFile = getFile(square);
        Rank squareRank = getRank(square);

        for (Rank rank = 0; rank < 8; ++rank) {
            for (File file = 0; file < 8; ++file) {
                Square possibleSquare = createSquare(file, rank);

                if (file - rank == squareFile - squareRank) diag |= squareBB(possibleSquare);
                if (file + rank == squareFile + squareRank) antiDiag |= squareBB(possibleSquare);
            }
        }

        diagBB[square]     = diag;
        antiDiagBB[square] = antiDiag;
    }
}