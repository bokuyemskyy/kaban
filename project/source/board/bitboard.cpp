#include "bitboard.hpp"

std::array<Bitboard, Ranks::NB> rankBB{};
std::array<Bitboard, Files::NB> fileBB{};

void initBitboards() {
    constexpr Bitboard RANK_A = 0xFFULL;
    for (Rank rank = 0; rank <= Ranks::LAST; ++rank) rankBB[rank] = RANK_A << (rank * Squares::NB);

    constexpr Bitboard FILE_A = 0x0101010101010101ULL;
    for (File file = 0; file <= Files::LAST; ++file) fileBB[file] = FILE_A << file;
}