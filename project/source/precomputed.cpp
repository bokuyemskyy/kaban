#include "precomputed.hpp"

std::array<std::array<Bitboard, SQUARE_NB>, COLOR_NB> pawnAttacks{};
std::array<std::array<Bitboard, SQUARE_NB>, PIECETYPE_NB> pseudoAttacks{};

void precomputeAttacks() {
    for (Square s = Square::FIRST; s <= Square::LAST; ++s) {
        for (auto direction :
             {Direction(1, 0), Direction(0, 1), Direction(-1, 0), Direction(0, -1), Direction(1, 1),
              Direction(-1, 1), Direction(-1, -1), Direction(1, -1)}) {
            if (s + direction != Square::NONE)
                pseudoAttacks[static_cast<u8>(PieceType::KING)][static_cast<u8>(s)] |=
                    1ULL << static_cast<u8>(s + direction);
        }
    }
}