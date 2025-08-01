#ifndef MAGIC_HPP
#define MAGIC_HPP

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>

#include "bitboard.hpp"
#include "navigation.hpp"
#include "piece.hpp"
#include "utils.hpp"
using Shift = uint8_t;

struct Magic {
    Bitboard magic;
    Shift    shift{};
    Bitboard premask;
    size_t   attacksOffset{};

    [[nodiscard]] constexpr size_t index(Bitboard occupancy) const {
        return (((occupancy & premask) * magic) >> shift);
    }
};

constexpr size_t ROOK_ATTACKS_SIZE   = 102400;
constexpr size_t BISHOP_ATTACKS_SIZE = 5248;
constexpr size_t TOTAL_ATTACKS_SIZE  = ROOK_ATTACKS_SIZE + BISHOP_ATTACKS_SIZE;

class MagicGenerator {
    static constexpr int MAX_OCCUPANCIES = 4096;

    class PRNG {
        uint64_t s;

        constexpr uint64_t rand64() {
            s ^= s >> 12, s ^= s << 25, s ^= s >> 27;
            return s * 2685821657736338717LL;
        }

       public:
        constexpr PRNG(uint64_t seed) : s(seed) { assert(seed); }

        template <typename T>
        constexpr T rand() {
            return T(rand64());
        }

        template <typename T>
        constexpr T sparse_rand() {
            return T(rand64() & rand64() & rand64());
        }
    };

    static constexpr Bitboard premask(PieceType pieceType, Square square) {
        switch (pieceType) {
            case PieceType::ROOK: {
                // Board edges are not considered in the relevant occupancies
                Bitboard edges =
                    ((Bitboard::rankBB(Rank::R1) | Bitboard::rankBB(Rank::R8)) & ~Bitboard::rankBB(square.rank())) |
                    ((Bitboard::fileBB(File::FA) | Bitboard::fileBB(File::FH)) & ~Bitboard::fileBB(square.file()));
                return slidingAttacksBB(PieceType::ROOK, square, Bitboard::zero()) & ~edges;
            }
            case PieceType::BISHOP: {
                Bitboard edges = (Bitboard::rankBB(Rank::R1) | Bitboard::rankBB(Rank::R8) | Bitboard::fileBB(File::FA) |
                                  Bitboard::fileBB(File::FH));
                return slidingAttacksBB(PieceType::BISHOP, square, Bitboard::zero()) & ~edges;
            }
            default:
                return Bitboard::zero();
        }
    }

    static constexpr Bitboard slidingAttacksBB(PieceType pieceType, Square square, Bitboard occupancy) {
        Bitboard attacks = Bitboard::zero();

        constexpr std::array<Direction, 4> rookDirections   = {Direction::N, Direction::S, Direction::E, Direction::W};
        constexpr std::array<Direction, 4> bishopDirections = {Direction::NE, Direction::NW, Direction::SE,
                                                               Direction::SW};

        const auto& directions = (pieceType == PieceType::BISHOP) ? bishopDirections : rookDirections;

        for (Direction direction : directions) {
            Square currentSquare = square;

            while (true) {
                Bitboard destination = Bitboard::destinationBB(currentSquare, direction);
                if (destination == Bitboard::zero()) break;

                attacks |= destination;
                currentSquare = currentSquare + direction;

                if ((destination & occupancy) != Bitboard::zero()) {
                    break;
                }
            }
        }

        return attacks;
    }

   public:
    static constexpr auto generateMagics() {
        std::array<Magic, Square::number()>      rookMagics{};
        std::array<Magic, Square::number()>      bishopMagics{};
        std::array<Bitboard, TOTAL_ATTACKS_SIZE> attacks{};

        // Optimal PRNG seeds from Stockfish to pick the correct magics in the shortest time
        constexpr std::array<std::array<int, 8>, 2> seeds = {{
            {8977, 44560, 54343, 38998, 5731, 95205, 104912, 17020},  // Rook seeds
            {728, 10316, 55013, 32803, 12281, 15100, 16645, 255}      // Bishop seeds
        }};

        size_t attacksOffset = 0;

        // Generate rook magics
        for (auto square : Square::all()) {
            Magic& magic        = rookMagics[square];
            magic.premask       = premask(PieceType::ROOK, square);
            magic.shift         = Square::number() - popCount(magic.premask);
            magic.attacksOffset = attacksOffset;

            std::array<Bitboard, MAX_OCCUPANCIES> occupancies{};
            std::array<Bitboard, MAX_OCCUPANCIES> reference{};
            std::array<int, MAX_OCCUPANCIES>      epoch{};

            int size = 0;
            int cnt  = 0;

            // Use Carry-Rippler trick to enumerate all subsets of mask
            Bitboard b = Bitboard::zero();
            do {
                occupancies[size] = b;
                reference[size]   = slidingAttacksBB(PieceType::ROOK, square, b);
                size++;
                b = (b - magic.premask) & magic.premask;
            } while (b != Bitboard::zero());

            PRNG rng(static_cast<uint64_t>(seeds[0][square.rank()]));

            // Find a magic for square picking up an (almost) random number
            // until we find the one that passes the verification test
            for (int i = 0; i < size;) {
                // Generate sparse magic numbers
                for (magic.magic = Bitboard::zero(); popCount((magic.magic * magic.premask) >> 56) < 6;) {
                    magic.magic = rng.sparse_rand<Bitboard>();
                }

                // A good magic must map every possible occupancy to an index that
                // looks up the correct sliding attack in the attacks database.
                // Note that we build up the database for square as a side
                // effect of verifying the magic. Keep track of the attempt count
                // and save it in epoch[], little speed-up trick to avoid resetting
                // attacks[] after every failed attempt.
                for (++cnt, i = 0; i < size; ++i) {
                    size_t idx = magic.index(occupancies[i]);
                    if (epoch[idx] < cnt) {
                        epoch[idx]                   = cnt;
                        attacks[attacksOffset + idx] = reference[i];
                    } else if (attacks[attacksOffset + idx] != reference[i]) {
                        break;
                    }
                }
            }

            attacksOffset += (1ULL << popCount(magic.premask));
        }

        for (auto square : Square::all()) {
            Magic& magic        = bishopMagics[square];
            magic.premask       = premask(PieceType::BISHOP, square);
            magic.shift         = Square::number() - popCount(magic.premask);
            magic.attacksOffset = attacksOffset;

            std::array<Bitboard, MAX_OCCUPANCIES> occupancies{};
            std::array<Bitboard, MAX_OCCUPANCIES> reference{};
            std::array<int, MAX_OCCUPANCIES>      epoch{};

            int size = 0;
            int cnt  = 0;

            Bitboard b = Bitboard::zero();
            do {
                occupancies[size] = b;
                reference[size]   = slidingAttacksBB(PieceType::BISHOP, square, b);
                size++;
                b = (b - magic.premask) & magic.premask;
            } while (b != Bitboard::zero());

            PRNG rng(static_cast<uint64_t>(seeds[1][square.rank()]));

            for (int i = 0; i < size;) {
                for (magic.magic = Bitboard::zero(); popCount((magic.magic * magic.premask) >> 56) < 6;) {
                    magic.magic = rng.sparse_rand<Bitboard>();
                }

                for (++cnt, i = 0; i < size; ++i) {
                    size_t idx = magic.index(occupancies[i]);
                    if (epoch[idx] < cnt) {
                        epoch[idx]                   = cnt;
                        attacks[attacksOffset + idx] = reference[i];
                    } else if (attacks[attacksOffset + idx] != reference[i]) {
                        break;
                    }
                }
            }

            attacksOffset += (1ULL << popCount(magic.premask));
        }

        return std::make_tuple(rookMagics, bishopMagics, attacks);
    }
};

auto            MAGIC_TABLES  = MagicGenerator::generateMagics();
constexpr auto& ROOK_MAGICS   = std::get<0>(MAGIC_TABLES);
constexpr auto& BISHOP_MAGICS = std::get<1>(MAGIC_TABLES);
constexpr auto& MAGIC_ATTACKS = std::get<2>(MAGIC_TABLES);

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

#endif