#pragma once

#include <array>
#include <cassert>
#include <cstddef>

#include "bit_operations.hpp"
#include "bitboard.hpp"
#include "direction.hpp"
#include "piece_type.hpp"
#include "square.hpp"

class Magics {
   public:
    Magics() {
        generate(PieceTypes::BISHOP, bishop_magics, bishop_attacks);
        generate(PieceTypes::ROOK, rook_magics, rook_attacks);
    }

    template <PieceType PT>
    [[nodiscard]] constexpr Bitboard lookup(Square square, Bitboard occupancy) const {
        assert(PT == PieceTypes::BISHOP || PT == PieceTypes::ROOK);
        if constexpr (PT == PieceTypes::BISHOP) {
            const auto& entry = bishop_magics[square.value()];
            size_t      index = entry.index(occupancy);
            return bishop_attacks[entry.attacks_offset + index];
        } else {
            const auto& entry = rook_magics[square.value()];
            size_t      index = entry.index(occupancy);
            return rook_attacks[entry.attacks_offset + index];
        }
    }

   private:
    using Shift = uint8_t;
    using Magic = uint64_t;

    struct MagicEntry {
        Magic    magic;
        Shift    shift{};
        Bitboard premask;
        size_t   attacks_offset{};

        [[nodiscard]] constexpr size_t index(Bitboard occupancy) const {
            return (((occupancy & premask).value() * magic) >> shift);
        }
    };

    class Prng {
        uint64_t s;

        constexpr uint64_t rand64() {
            s ^= s >> 12, s ^= s << 25, s ^= s >> 27;
            return s * 2685821657736338717LL;
        }

       public:
        constexpr Prng(uint64_t seed) : s(seed) { assert(seed); }

        template <typename T>
        constexpr T rand() {
            return T(rand64());
        }

        template <typename T>
        constexpr T sparseRand() {
            return T(rand64() & rand64() & rand64());
        }
    };

    static constexpr int    MAX_OCCUPANCIES     = 4096;
    static constexpr size_t ROOK_ATTACKS_SIZE   = 102400;
    static constexpr size_t BISHOP_ATTACKS_SIZE = 5248;

    std::array<MagicEntry, Squares::count()> bishop_magics{};
    std::array<MagicEntry, Squares::count()> rook_magics{};

    std::array<Bitboard, BISHOP_ATTACKS_SIZE> bishop_attacks{};
    std::array<Bitboard, ROOK_ATTACKS_SIZE>   rook_attacks{};

    constexpr Bitboard premask(PieceType piece_type, Square square) {
        assert(piece_type == PieceTypes::BISHOP || piece_type == PieceTypes::ROOK);

        Bitboard edges;
        if (piece_type == PieceTypes::BISHOP) {
            edges = (Bitboard::rank(Ranks::R1) | Bitboard::rank(Ranks::R8) | Bitboard::file(Files::FA) |
                     Bitboard::file(Files::FH));

        } else {
            edges = ((Bitboard::rank(Ranks::R1) | Bitboard::rank(Ranks::R8)) & ~Bitboard::rank(square.rank())) |
                    ((Bitboard::file(Files::FA) | Bitboard::file(Files::FH)) & ~Bitboard::file(square.file()));
        }

        return Bitboard::slidingAttacks(square, Directions::of(piece_type), Bitboards::ZERO) & ~edges;
    }

    template <size_t N>
    constexpr void generate(PieceType piece_type, std::array<MagicEntry, Squares::count()>& magics,
                            std::array<Bitboard, N>& attacks) {
        assert(piece_type == PieceTypes::BISHOP || piece_type == PieceTypes::ROOK);
        static_assert(sizeof(void*) == 8, "Must be 64-bit build");
        // optimal PRNG seeds of Stockfish
        // to pseudo randomly generate magics in the shortest time
        constexpr std::array<uint64_t, 8> seeds = {728, 10316, 55013, 32803, 12281, 15100, 16645, 255};

        size_t attacks_offset = 0;

        for (auto square : Squares::all()) {
            MagicEntry& magic_entry    = magics[square.value()];
            magic_entry.premask        = premask(piece_type, square);
            magic_entry.shift          = Squares::count() - popcount(magic_entry.premask);
            magic_entry.attacks_offset = attacks_offset;

            std::array<Bitboard, MAX_OCCUPANCIES> occupancies{};
            std::array<Bitboard, MAX_OCCUPANCIES> reference{};
            std::array<int, MAX_OCCUPANCIES>      epoch{};

            size_t size  = 0;
            int    count = 0;

            // use Carry-Rippler method
            Bitboard possible_occupancy = Bitboards::ZERO;
            do {
                occupancies[size] = possible_occupancy;
                reference[size]   = Bitboard::slidingAttacks(square, Directions::of(piece_type), possible_occupancy);
                size++;
                possible_occupancy =
                    Bitboard(possible_occupancy.value() - magic_entry.premask.value()) & magic_entry.premask;
            } while (possible_occupancy != Bitboards::ZERO);

            Prng rng(seeds[square.rank().value()]);

            for (size_t i = 0; i < size;) {
                for (magic_entry.magic = 0;
                     popcount(Bitboard(magic_entry.magic * magic_entry.premask.value()) >> 56) < 6;) {
                    magic_entry.magic = rng.sparseRand<Magic>();
                }
                for (++count, i = 0; i < size; ++i) {
                    size_t index = magic_entry.index(occupancies[i]);
                    if (epoch[index] < count) {
                        epoch[index]                    = count;
                        attacks[attacks_offset + index] = reference[i];
                    } else if (attacks[attacks_offset + index] != reference[i]) {
                        break;
                    }
                }
            }

            attacks_offset += (1ULL << popcount(magic_entry.premask));
        }
    }
};

/*
constexpr Bitboard rookAttacksBB(Square square, Bitboard occupancy) {
    const Magic& magic = ROOK_MAGICS[square];
    return MAGIC_ATTACKS[magic.attacksOffset + magic.index(occupancy)];
}

constexpr Bitboard bishopAttacksBB(Square square, Bitboard occupancy) {
    const Magic& magic = BISHOP_MAGICS[square];
    return MAGIC_ATTACKS[magic.attacksOffset + magic.index(occupancy)];
}

constexpr Bitboard queenAttacksBB(Square square, Bitboard occupancy) {
    return rookAttacksBB(square, occupancy) | bishopAttacksBB(square, occupancy);
}
*/