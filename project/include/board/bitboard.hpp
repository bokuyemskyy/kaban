#ifndef BITBOARD_HPP
#define BITBOARD_HPP

#include <array>
#include <cassert>
#include <cstdint>

#include "navigation.hpp"

struct Bitboard {
   public:
    constexpr Bitboard(uint64_t value = zero()) : m_value(value) {};

    constexpr void                   set(uint64_t value) { m_value = value; }
    [[nodiscard]] constexpr uint64_t value() const { return m_value; }

    constexpr Bitboard& operator|=(const Bitboard& other) {
        m_value |= other.m_value;
        return *this;
    }
    constexpr Bitboard& operator&=(const Bitboard& other) {
        m_value &= other.m_value;
        return *this;
    }
    constexpr Bitboard& operator^=(const Bitboard& other) {
        m_value ^= other.m_value;
        return *this;
    }

    constexpr operator uint64_t() const { return m_value; }

    constexpr bool operator==(const Bitboard& other) const { return m_value == other.m_value; }
    constexpr bool operator!=(const Bitboard& other) const { return m_value != other.m_value; }

    static constexpr Bitboard zero() { return 0ULL; }

    static constexpr Bitboard squareBB(Square square) { return (1ULL << square); }
    static constexpr Bitboard destinationBB(Square square, Direction direction) {
        Square destination = square + direction;
        return (destination.ok() && Square::distance(square, destination) <= 2) ? squareBB(destination) : zero();
    };

    template <int8_t D>
    [[nodiscard]] constexpr Bitboard pushed() const {
        switch (D) {
            case Direction::N:
            case Direction::NN:
                return m_value << D;
            case Direction::S:
            case Direction::SS:
                return m_value >> -D;
            case Direction::NE:
            case Direction::SE:
                return (m_value & ~fileBB(File::FH)) << D;
            case Direction::NW:
            case Direction::SW:
                return (m_value & ~fileBB(File::FA)) >> -D;
            default:
                return zero();
        }
    }

    static constexpr Bitboard rankBB(Rank rank) {
        static constexpr auto table = []() constexpr {
            std::array<Bitboard, Rank::number()> t{};

            constexpr Bitboard RANK_A = 0xFFULL;

            for (auto rank : Rank::all()) {
                t[rank] = (RANK_A << (rank * File::number()));
            }
            return t;
        }();

        return table[rank];
    }

    static constexpr Bitboard fileBB(File file) {
        static constexpr auto table = []() constexpr {
            std::array<Bitboard, File::NB> t{};

            constexpr Bitboard FILE_A = 0x0101010101010101ULL;

            for (auto file : File::all()) {
                t[file] = (FILE_A << file);
            }
            return t;
        }();

        return table[file];
    }

    static constexpr Bitboard diagBB(Square square) {
        static constexpr auto table = []() constexpr {
            std::array<Bitboard, Square::NB> t{};
            for (auto square : Square::all()) {
                Bitboard diag       = zero();
                File     squareFile = square.file();
                Rank     squareRank = square.rank();

                for (auto rank : Rank::all()) {
                    for (auto file : File::all()) {
                        Square possibleSquare = Square(file, rank);
                        if (file - rank == squareFile - squareRank) {
                            diag = diag | squareBB(possibleSquare);
                        }
                    }
                }
                t[square] = diag;
            }
            return t;
        }();
        return table[square];
    }

    static constexpr Bitboard antiDiagBB(Square square) {
        static constexpr auto table = []() constexpr {
            std::array<Bitboard, Square::NB> t{};
            for (auto square : Square::all()) {
                Bitboard antiDiag   = zero();
                File     squareFile = square.file();
                Rank     squareRank = square.rank();

                for (auto rank : Rank::all()) {
                    for (auto file : File::all()) {
                        Square possibleSquare = Square(file, rank);
                        if (file + rank == squareFile + squareRank) {
                            antiDiag = antiDiag | squareBB(possibleSquare);
                        }
                    }
                }
                t[square] = antiDiag;
            }
            return t;
        }();
        return table[square];
    }

   private:
    uint64_t m_value;
};
#endif
