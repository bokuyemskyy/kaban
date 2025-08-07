#pragma once

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

    [[nodiscard]] constexpr bool empty() const { return m_value == zero(); }

    static constexpr Bitboard square(Square square) { return (1ULL << square.value()); }
    static constexpr Bitboard destination(Square square, Direction direction) {
        Square destination = square + direction;
        return (destination.isOk() && Square::distance(square, destination) <= 2) ? Bitboard::square(destination)
                                                                                  : zero();
    };

    static constexpr Bitboard rank(Rank rank) {
        static constexpr auto table = []() constexpr {
            std::array<Bitboard, Rank::number()> t{};

            constexpr Bitboard RANK_A = 0xFFULL;

            for (auto rank : Ranks::all()) {
                t[rank.value()] = (RANK_A << (rank.value() * File::number()));
            }
            return t;
        }();

        return table[rank.value()];
    }

    static constexpr Bitboard file(File file) {
        static constexpr auto table = []() constexpr {
            std::array<Bitboard, File::number()> t{};

            constexpr Bitboard FILE_A = 0x0101010101010101ULL;

            for (auto file : Files::all()) {
                t[file.value()] = (FILE_A << file.value());
            }
            return t;
        }();

        return table[file.value()];
    }

    static constexpr Bitboard diag(Square square) {
        static constexpr auto table = []() constexpr {
            std::array<Bitboard, Square::number()> t{};
            for (auto square : Squares::all()) {
                Bitboard diag       = zero();
                File     squareFile = square.file();
                Rank     squareRank = square.rank();

                for (auto rank : Ranks::all()) {
                    for (auto file : Files::all()) {
                        Square possibleSquare = Square(file, rank);
                        if (file.value() - rank.value() == squareFile.value() - squareRank.value()) {
                            diag = diag | Bitboard::square(possibleSquare);
                        }
                    }
                }
                t[square.value()] = diag;
            }
            return t;
        }();
        return table[square.value()];
    }

    static constexpr Bitboard antiDiag(Square square) {
        static constexpr auto table = []() constexpr {
            std::array<Bitboard, Square::number()> t{};
            for (auto square : Squares::all()) {
                Bitboard antiDiag   = zero();
                File     squareFile = square.file();
                Rank     squareRank = square.rank();

                for (auto rank : Ranks::all()) {
                    for (auto file : Files::all()) {
                        Square possibleSquare = Square(file, rank);
                        if (file.value() + rank.value() == squareFile.value() + squareRank.value()) {
                            antiDiag = antiDiag | Bitboard::square(possibleSquare);
                        }
                    }
                }
                t[square.value()] = antiDiag;
            }
            return t;
        }();
        return table[square.value()];
    }

    /*template <int8_t D>
    [[nodiscard]] constexpr Bitboard shifted() const {
        switch (D) {
            case Direction::N:
            case Direction::NN:
                return m_value << D;
            case Direction::S:
            case Direction::SS:
                return m_value >> -D;
            case Direction::NE:
            case Direction::SE:
                return (m_value & ~Bitboard::file(File::FH)) << D;
            case Direction::NW:
            case Direction::SW:
                return (m_value & ~Bitboard::file(File::FA)) >> -D;
            default:
                return zero();
        }
    }*/

   private:
    uint64_t m_value;
};