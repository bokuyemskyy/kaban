#pragma once

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>

#include "bitboard.hpp"
#include "bitops.hpp"
#include "navigation.hpp"
#include "piece.hpp"

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

class Magics {
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
                Bitboard edges =
                    ((Bitboard::rank(Rank::R1) | Bitboard::rank(Rank::R8)) & ~Bitboard::rank(square.rank())) |
                    ((Bitboard::file(File::FA) | Bitboard::file(File::FH)) & ~Bitboard::file(square.file()));
                return slidingAttacksBB(PieceType::ROOK, square, Bitboard::zero()) & ~edges;
            }
            case PieceType::BISHOP: {
                Bitboard edges = (Bitboard::rank(Rank::R1) | Bitboard::rank(Rank::R8) | Bitboard::file(File::FA) |
                                  Bitboard::file(File::FH));
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
                Bitboard destination = Bitboard::destination(currentSquare, direction);
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
    static auto generateMagics() {
        std::array<Magic, Square::number()>      rookMagics{};
        std::array<Magic, Square::number()>      bishopMagics{};
        std::array<Bitboard, TOTAL_ATTACKS_SIZE> attacks{};

        // optimal PRNG seeds from Stockfish to pick the correct magics in the shortest time
        constexpr std::array<std::array<int, 8>, 2> seeds = {{{8977, 44560, 54343, 38998, 5731, 95205, 104912, 17020},
                                                              {728, 10316, 55013, 32803, 12281, 15100, 16645, 255}}};

        size_t attacksOffset = 0;

        for (auto square : Square::all()) {
            Magic& magic        = rookMagics[square];
            magic.premask       = premask(PieceType::ROOK, square);
            magic.shift         = Square::number() - popcount(magic.premask);
            magic.attacksOffset = attacksOffset;

            std::array<Bitboard, MAX_OCCUPANCIES> occupancies{};
            std::array<Bitboard, MAX_OCCUPANCIES> reference{};
            std::array<int, MAX_OCCUPANCIES>      epoch{};

            size_t size = 0;
            int    cnt  = 0;

            // use Carry-Rippler trick
            Bitboard b = Bitboard::zero();
            do {
                occupancies[size] = b;
                reference[size]   = slidingAttacksBB(PieceType::ROOK, square, b);
                size++;
                b = (b - magic.premask) & magic.premask;
            } while (b != Bitboard::zero());

            PRNG rng(static_cast<uint64_t>(seeds[0][square.rank()]));

            for (size_t i = 0; i < size;) {
                for (magic.magic = Bitboard::zero(); popcount((magic.magic * magic.premask) >> 56) < 6;) {
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

            attacksOffset += (1ULL << popcount(magic.premask));
        }

        for (auto square : Square::all()) {
            Magic& magic        = bishopMagics[square];
            magic.premask       = premask(PieceType::BISHOP, square);
            magic.shift         = Square::number() - popcount(magic.premask);
            magic.attacksOffset = attacksOffset;

            std::array<Bitboard, MAX_OCCUPANCIES> occupancies{};
            std::array<Bitboard, MAX_OCCUPANCIES> reference{};
            std::array<int, MAX_OCCUPANCIES>      epoch{};

            size_t size = 0;
            int    cnt  = 0;

            Bitboard b = Bitboard::zero();
            do {
                occupancies[size] = b;
                reference[size]   = slidingAttacksBB(PieceType::BISHOP, square, b);
                size++;
                b = (b - magic.premask) & magic.premask;
            } while (b != Bitboard::zero());

            PRNG rng(static_cast<uint64_t>(seeds[1][square.rank()]));

            for (size_t i = 0; i < size;) {
                for (magic.magic = Bitboard::zero(); popcount((magic.magic * magic.premask) >> 56) < 6;) {
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

            attacksOffset += (1ULL << popcount(magic.premask));
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
