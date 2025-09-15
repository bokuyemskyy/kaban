
#pragma once

#include <array>
#include <cassert>
#include <cstdint>
#include <span>

#include "square.hpp"
#include "strong_value.hpp"

class Bitboard : public StrongValue<Bitboard, uint64_t> {
   public:
    using StrongValue::StrongValue;

    [[nodiscard]] constexpr bool hasValue() const { return *this != Bitboard(0); }

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
                Bitboard diag        = Bitboard(0);
                File     square_file = square.file();
                Rank     square_rank = square.rank();

                for (auto rank : Ranks::all()) {
                    for (auto file : Files::all()) {
                        Square possible_square = Square(file, rank);
                        if (file.value() - rank.value() == square_file.value() - square_rank.value()) {
                            diag = diag | Bitboard::square(possible_square);
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
            std::array<Bitboard, Square::count()> t{};
            for (auto square : Squares::all()) {
                Bitboard anti_diag   = Bitboard(0);
                File     square_file = square.file();
                Rank     square_rank = square.rank();

                for (auto rank : Ranks::all()) {
                    for (auto file : Files::all()) {
                        Square possible_square = Square(file, rank);
                        if (file.value() + rank.value() == square_file.value() + square_rank.value()) {
                            anti_diag = anti_diag | Bitboard::square(possible_square);
                        }
                    }
                }
                t[square.value()] = anti_diag;
            }
            return t;
        }();
        return table[square.value()];
    }

    static constexpr std::array<Bitboard, Square::count()> pseudoAttacks(const std::span<const Direction>& directions) {
        std::array<Bitboard, Square::count()> table{};

        for (auto square : Squares::all()) {
            Bitboard pseudo_attacks = Bitboard(0);
            for (const auto& direction : directions) {
                Square destination = square.shifted(direction);
                if (destination.hasValue()) pseudo_attacks |= Bitboard::square(destination);
            }
            table[square.value()] = pseudo_attacks;
        }

        return table;
    }

    static constexpr Bitboard slidingAttacks(Square from, const std::span<const Direction>& directions,
                                             Bitboard occupancy) {
        Bitboard attacks = Bitboard(0);

        for (const auto& direction : directions) {
            Square square = from;
            while (true) {
                square = square.shifted(direction);
                if (!square.hasValue()) break;
                attacks |= Bitboard::square(square);
                if ((occupancy & Bitboard::square(square)).hasValue()) break;
            }
        }
        return attacks;
    }
};

struct Bitboards {
    static constexpr Bitboard ZERO = Bitboard(0);
    static constexpr Bitboard ONE  = Bitboard(1);
};