#include "magic.hpp"

#include <sys/types.h>

#include <array>
#include <cstdint>
#include <cstring>
#include <random>
#include <vector>

#include "bitboard.hpp"
#include "square.hpp"
#include "utils.hpp"

std::array<Magic, Squares::NB> bishopMagics{};
std::array<Magic, Squares::NB> rookMagics{};

std::array<std::vector<Bitboard>, Squares::NB> bishopOccupancies{};
std::array<std::vector<Bitboard>, Squares::NB> rookOccupancies{};

std::mt19937_64 rng(std::random_device{}());

inline uint64_t randomMagic() { return rng() & rng() & rng(); }

Bitboard getRookPremask(Square square) {
    return ((fileBB[getFile(square)] & ~(rankBB[Ranks::R1] | rankBB[Ranks::R8])) |
            (rankBB[getRank(square)] & ~(fileBB[Files::FA] | fileBB[Files::FH]))) &
           ~squareBB(square);
}
Bitboard getBishopPremask(Square square) {
    return (diagBB[square] | antiDiagBB[square]) &
           ~(fileBB[Files::FA] | fileBB[Files::FH] | rankBB[Ranks::R1] | rankBB[Ranks::R8]) & ~squareBB(square);
}

Bitboard createOccupancy(int index, Bitboard premask) {
    Bitboard result = BITBOARD_ZERO;

    while (premask != BITBOARD_ZERO) {
        int bitPosition = popLsb(premask);
        if ((index & 0b1) != 0) {
            result |= squareBB(bitPosition);
        }
        index >>= 1;
    }

    return result;
}
void initOccupancies(Square square, Bitboard premask) {
    int bitsNB        = popcount(premask);
    int occupanciesNB = 1 << bitsNB;

    rookOccupancies[square].reserve(occupanciesNB);

    for (int i = 0; i < occupanciesNB; ++i) {
        rookOccupancies[square].emplace_back(createOccupancy(i, premask));
    }
}

void initMagics() {
    for (Square square = Squares::FIRST; square <= Squares::LAST; ++square) {
        Bitboard premask = getRookPremask(square);

        initOccupancies(square, premask);

        uint8_t bitsNB = popcount(premask);
        Shift   shift  = Squares::NB - bitsNB;

        int used[4096]{};
        int epoch = 0;

        while (true) {
            Bitboard magic  = randomMagic();
            bool     failed = false;

            ++epoch;

            for (const auto& occupancy : rookOccupancies[square]) {
                int index = (occupancy * magic) >> shift;

                if (used[index] != epoch) {
                    used[index] = epoch;
                } else {
                    failed = true;
                    break;
                }
            }

            if (!failed) {
                rookMagics[square] = {.magic = magic, .shift = shift, .premask = premask};
                break;
            }
        }
    }
}
