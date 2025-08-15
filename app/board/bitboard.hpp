#pragma once

#include <array>
#include <cassert>
#include <cstdint>

#include "square.hpp"
#include "strong.hpp"

class Bitboard : public Strong::Value<Bitboard, uint64_t> {
   public:
    using Value::Value;

    [[nodiscard]] constexpr bool hasValue() const { return m_value != Values::ZERO; }

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

    static constexpr Bitboard square(Square square) { return Bitboard(Values::ONE) << square.value(); }

    static constexpr Bitboard rank(Rank rank) {
        static constexpr auto table = []() constexpr {
            std::array<Bitboard, Rank::number()> t{};

            constexpr Bitboard RANK_A = Bitboard(0xFFULL);

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
            std::array<Bitboard, Square::number()> t{};

            for (auto square : Squares::all()) {
                Bitboard diag       = Bitboard(Values::ZERO);
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
                Bitboard antiDiag   = Bitboard(Values::ZERO);
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

   private:
    enum Values : ValueType {
        ZERO = 0,
        ONE  = 1
    };

    friend struct Bitboards;
};

struct Bitboards {
    static constexpr Bitboard ZERO = Bitboard(Bitboard::Values::ZERO);
};