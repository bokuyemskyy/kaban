#include "bitboard.hpp"

#include "square.hpp"

namespace Bitboards {

std::array<Bitboard, Ranks::NB> rankBBs{};
std::array<Bitboard, Files::NB> fileBBs{};

std::array<Bitboard, Squares::NB> diagBBs{};
std::array<Bitboard, Squares::NB> antiDiagBBs{};

void init() {
    constexpr Bitboard RANK_A = 0xFFULL;
    for (Rank rank = 0; rank <= Ranks::LAST; ++rank) rankBBs[rank] = (RANK_A << (rank * Files::NB));

    constexpr Bitboard FILE_A = 0x0101010101010101ULL;
    for (File file = 0; file <= Files::LAST; ++file) fileBBs[file] = FILE_A << file;

    for (Square square = Squares::FIRST; square <= Squares::LAST; ++square) {
        Bitboard diag = ZERO, antiDiag = ZERO;

        File squareFile = getFile(square);
        Rank squareRank = getRank(square);

        for (Rank rank = 0; rank < 8; ++rank) {
            for (File file = 0; file < 8; ++file) {
                Square possibleSquare = createSquare(file, rank);

                if (file - rank == squareFile - squareRank) diag |= squareBB(possibleSquare);
                if (file + rank == squareFile + squareRank) antiDiag |= squareBB(possibleSquare);
            }
        }

        diagBBs[square]     = diag;
        antiDiagBBs[square] = antiDiag;
    }
}

}  // namespace Bitboards