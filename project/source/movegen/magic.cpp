#include "magic.hpp"

#include <sys/types.h>

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <vector>

#include "bitboard.hpp"
#include "piece.hpp"
#include "square.hpp"
#include "utils.hpp"

namespace Movegen {

alignas(64) std::array<std::array<Magic, Squares::NB>, 2> magics{};

class PRNG {
    uint64_t s;
    uint64_t rand64() {
        s ^= s >> 12, s ^= s << 25, s ^= s >> 27;
        return s * 2685821657736338717LL;
    }

   public:
    PRNG(uint64_t seed) : s(seed) { assert(seed); }
    template <typename T>
    T rand() {
        return T(rand64());
    }
    template <typename T>
    T sparse_rand() {
        return T(rand64() & rand64() & rand64());
    }
};
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

Bitboard createOccupancy(size_t index, Bitboard premask) {
    Bitboard result = Bitboards::ZERO;

    while (premask != Bitboards::ZERO) {
        int bitPosition = popLSB(premask);
        if ((index & 0b1) != 0) {
            result |= squareBB(bitPosition);
        }
        index >>= 1;
    }

    return result;
}

void fillOccupancies(std::vector<Bitboard>& occupancies, Bitboard premask) {
    int    bitsNB        = popCount(premask);
    size_t occupanciesNB = 1 << bitsNB;

    occupancies.clear();
    occupancies.reserve(occupanciesNB);

    for (size_t i = 0; i < occupanciesNB; ++i) {
        occupancies[i] = createOccupancy(i, premask);
    }
}

void initMagics(PieceType pieceType, std::vector<Bitboard>* pieceAttacks) {
    // optimal PRNG seeds to pick the correct magics in the shortest time, stolen from Stockfish
    int seeds[][Ranks::NB] = {{8977, 44560, 54343, 38998, 5731, 95205, 104912, 17020},
                              {728, 10316, 55013, 32803, 12281, 15100, 16645, 255}};

    // occupancies is a buffer vector of all possible combinations of bits of a current premask
    // possible attack bitboards that correspond to each occupancy
    std::vector<Bitboard> occupancies, attacks;

    int used[ROOK_MAX_OCCUPANCY_NB] = {}, epoch = 0;

    for (Square square = Squares::FIRST; square <= Squares::LAST; ++square) {
        Magic& magic = magics[2][square];

        magic.premask = getPremask(pieceType, square);
        magic.shift   = Squares::NB - popCount(magic.premask);
        magic.attacks = square == Squares::A1
                            ? pieceAttacks->data()
                            : magics[pieceType - PieceTypes::BISHOP][square - 1].attacks + occupancies.size();

        fillOccupancies(occupancies, magic.premask);

        PRNG rng(static_cast<uint64_t>(seeds[1][getRank(square)]));

        for (size_t i = 0; i < occupancies.size();) {
            do {
                magic.magic = rng.sparse_rand<Bitboard>();
            } while (popCount((magic.magic * magic.premask) >> 56) < 6);

            for (++epoch, i = 0; i < occupancies.size(); ++i) {
                size_t index = magic.index(occupancies[i]);

                if (used[index] < epoch) {
                    used[index]          = epoch;
                    magic.attacks[index] = reference[i];
                } else if (m.attacks[idx] != reference[i]) {
                    break;  // collision, try new magic

                    ///////// FIX
                }
            }
        }
    }
}

}  // namespace Movegen