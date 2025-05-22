#include "magic.hpp"

#include <sys/types.h>

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <random>
#include <vector>

#include "bitboard.hpp"
#include "piece.hpp"
#include "square.hpp"
#include "utils.hpp"

std::array<std::array<Magic, Squares::NB>, 2> magics{};

// all possible blocker occupancy bitboards
// within that square’s relevant premask
std::array<std::array<std::vector<Bitboard>, Squares::NB>, 2> premaskOccupancies{};

uint64_t seed = 0xdeadbeef12345678;

uint64_t xorshift64(uint64_t& state) {
    state ^= state >> 12;
    state ^= state << 25;
    state ^= state >> 27;
    return state * 2685821657736338717ULL;
}

inline uint64_t randomMagic() { return xorshift64(seed) & xorshift64(seed) & xorshift64(seed); }

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

void initPremaskOccupancies(PieceType pieceType, Square square, Bitboard premask) {
    int    bitsNB        = popcount(premask);
    size_t occupanciesNB = 1 << bitsNB;

    premaskOccupancies[pieceType - PieceTypes::BISHOP][square].reserve(occupanciesNB);

    for (size_t i = 0; i < occupanciesNB; ++i) {
        premaskOccupancies[pieceType - PieceTypes::BISHOP][square].emplace_back(createOccupancy(i, premask));
    }
}

void initMagics() {
    for (PieceType pieceType = PieceTypes::BISHOP; pieceType <= PieceTypes::ROOK; pieceType++) {
        for (Square square = Squares::FIRST; square <= Squares::LAST; ++square) {
            Bitboard premask = getPremask(pieceType, square);

            initPremaskOccupancies(pieceType, square, premask);

            uint8_t bitsNB = popcount(premask);
            Shift   shift  = Squares::NB - bitsNB;

            uint16_t used[ROOK_MAX_OCCUPANCY_NB]{};
            uint16_t epoch = 0;

            while (true) {
                Bitboard magic  = randomMagic();
                bool     failed = false;

                ++epoch;

                auto occupancies = getPremaskOccupancies(pieceType, square);
                for (const auto& occupancy : occupancies) {
                    uint16_t index = (occupancy * magic) >> shift;

                    if (used[index] != epoch) {
                        used[index] = epoch;
                    } else {
                        failed = true;
                        break;
                    }
                }

                if (!failed && (popcount((magic * premask) & 0xFF00000000000000ULL) < 6)) {
                    setMagic(pieceType, square, {.magic = magic, .shift = shift, .premask = premask});
                    break;
                }
            }
        }
    }
}
