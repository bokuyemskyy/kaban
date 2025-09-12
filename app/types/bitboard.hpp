
#pragma once

#include <array>
#include <cassert>
#include <cstdint>

#include "square.hpp"
#include "strong_value.hpp"

class Bitboard : public StrongValue<Bitboard, uint64_t> {
   public:
    using StrongValue::StrongValue;

    [[nodiscard]] constexpr bool has_value() const { return *this != Bitboard(0); }

    constexpr Bitboard operator~() const { return Bitboard(~m_value); }

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
    constexpr Bitboard& operator<<=(unsigned int n) {
        m_value <<= n;
        return *this;
    }
    constexpr Bitboard& operator>>=(unsigned int n) {
        m_value >>= n;
        return *this;
    }

    [[nodiscard]] constexpr Bitboard operator|(const Bitboard& other) const { return Bitboard(*this) |= other; }
    [[nodiscard]] constexpr Bitboard operator&(const Bitboard& other) const { return Bitboard(*this) &= other; }
    [[nodiscard]] constexpr Bitboard operator^(const Bitboard& other) const { return Bitboard(*this) ^= other; }
    [[nodiscard]] constexpr Bitboard operator<<(unsigned int n) const { return Bitboard(*this) <<= n; }
    [[nodiscard]] constexpr Bitboard operator>>(unsigned int n) const { return Bitboard(*this) >>= n; }

    static constexpr Bitboard square(Square square) { return Bitboard(1) << square.value(); }

    static constexpr Bitboard rank(Rank rank) {
        static constexpr auto table = []() constexpr {
            std::array<Bitboard, Rank::count()> t{};

            constexpr Bitboard RANK_A = Bitboard(0xFFULL);

            for (auto rank : Ranks::all()) {
                t[rank.value()] = (RANK_A << (rank.value() * File::count()));
            }
            return t;
        }();

        return table[rank.value()];
    }

    static constexpr Bitboard file(File file) {
        static constexpr auto table = []() constexpr {
            std::array<Bitboard, File::count()> t{};

            constexpr Bitboard FILE_A = Bitboard(0x0101010101010101ULL);

            for (auto file : Files::all()) {
                t[file.value()] = (FILE_A << file.value());
            }
            return t;
        }();

        return table[file.value()];
    }

    static constexpr Bitboard diag(Square square) {
        static constexpr auto table = []() constexpr {
            std::array<Bitboard, Square::count()> t{};

            for (auto square : Squares::all()) {
                Bitboard diag       = Bitboard(0);
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

    static constexpr Bitboard anti_diag(Square square) {
        static constexpr auto table = []() constexpr {
            std::array<Bitboard, Square::count()> t{};
            for (auto square : Squares::all()) {
                Bitboard antiDiag   = Bitboard(0);
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

    template <size_t N>
    static constexpr std::array<Bitboard, Square::count()> pseudo_attacks(const std::array<Direction, N>& directions) {
        std::array<Bitboard, Square::count()> table{};

        for (auto square : Squares::all()) {
            Bitboard pseudoAttacks = Bitboard(0);
            for (const auto& direction : directions) {
                Square destination = square.moved(direction);
                if (destination.has_value()) pseudoAttacks |= Bitboard::square(destination);
            }
            table[square.value()] = pseudoAttacks;
        }

        return table;
    }

    template <size_t N>
    static constexpr Bitboard sliding_attacks(Square from, const std::array<Direction, N>& directions,
                                              Bitboard occupancy) {
        Bitboard attacks = Bitboard(0);

        for (const auto& direction : directions) {
            Square square = from;
            while (true) {
                square.move(direction);
                if (!square.has_value()) break;
                attacks |= Bitboard::square(square);
                if ((occupancy & Bitboard::square(square)).has_value()) break;
            }
        }
        return attacks;
    }
};

struct Bitboards {
    static constexpr Bitboard ZERO = Bitboard(0);
    static constexpr Bitboard ONE  = Bitboard(1);
};