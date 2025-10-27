
#pragma once

#include <array>
#include <cassert>
#include <cstdint>
#include <span>

#include "direction.hpp"
#include "square.hpp"
#include "strong_value.hpp"

struct Bitboard : public StrongValue<Bitboard, uint64_t> {
    using StrongValue::StrongValue;

    [[nodiscard]] constexpr bool empty() const { return m_value == 0; }
    [[nodiscard]] constexpr bool any() const { return m_value != 0; }
    [[nodiscard]] constexpr bool test(Square square) const { return (*this & Bitboard::square(square)).any(); }

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
            std::array<Bitboard, Ranks::count()> t{};

            constexpr Bitboard RANK_A = Bitboard(0xFFULL);

            for (auto r : Ranks::all()) {
                t[r.value()] = (RANK_A << (r.value() * Files::count()));
            }
            return t;
        }();

        return table[rank.value()];
    }

    static constexpr Bitboard file(File file) {
        static constexpr auto table = []() constexpr {
            std::array<Bitboard, Files::count()> _table{};

            constexpr Bitboard FILE_A = Bitboard(0x0101010101010101ULL);

            for (auto _file : Files::all()) {
                _table[_file.value()] = (FILE_A << _file.value());
            }
            return _table;
        }();

        return table[file.value()];
    }

    static constexpr Bitboard diag(Square square) {
        static constexpr auto table = []() constexpr {
            std::array<Bitboard, Squares::count()> _table{};

            for (auto _square : Squares::all()) {
                Bitboard diag = Bitboard(0);
                
                for (auto rank : Ranks::all()) {
                    for (auto file : Files::all()) {
                        Square square_to_check = Square(file, rank);
                        if (square_to_check.diag() == _square.diag()) {
                            diag = diag | Bitboard::square(square_to_check);
                        }
                    }
                }

                _table[_square.value()] = diag;
            }
            return _table;
        }();
        return table[square.value()];
    }

    static constexpr Bitboard antiDiag(Square square) {
        static constexpr auto table = []() constexpr {
            std::array<Bitboard, Squares::count()> _table{};
            for (auto _square : Squares::all()) {
                Bitboard anti_diag = Bitboard(0);

                for (auto rank : Ranks::all()) {
                    for (auto file : Files::all()) {
                        Square square_to_check = Square(file, rank);
                        if (square_to_check.antiDiag() == _square.antiDiag()) {
                            anti_diag = anti_diag | Bitboard::square(square_to_check);
                        }
                    }
                }
                _table[_square.value()] = anti_diag;
            }
            return _table;
        }();
        return table[square.value()];
    }

    static constexpr std::array<Bitboard, Squares::count()> pseudoAttacks(
        const std::span<const Direction>& directions) {
        std::array<Bitboard, Squares::count()> table{};
        for (auto _square : Squares::all()) {
            Bitboard pseudo_attacks = Bitboard(0);
            for (const auto& direction : directions) {
                Square destination = _square.move(direction);
                if (destination != Squares::NONE) pseudo_attacks |= Bitboard::square(destination);
            }
            table[_square.value()] = pseudo_attacks;
        }

        return table;
    }

    static constexpr Bitboard slidingAttacks(Square from, std::span<const Direction> directions, Bitboard occupancy) {
        Bitboard attacks{};
        for (Direction direction : directions) {
            Square _square = from;
            while ((_square = _square.move(direction)) != Squares::NONE) {
                auto bitboard = Bitboard::square(_square);
                attacks |= bitboard;
                if ((occupancy & bitboard).any()) break;
            }
        }
        return attacks;
    }

    static constexpr Bitboard slidingAttacks(Square from, Direction direction, Bitboard occupancy) {
        Bitboard attacks{};

        Square _square = from;
        while ((_square = _square.move(direction)) != Squares::NONE) {
            auto bitboard = Bitboard::square(_square);
            attacks |= bitboard;
            if ((occupancy & bitboard).any()) break;
        }

        return attacks;
    }

    static constexpr Bitboard line(Square from, Square to) {
        static constexpr auto table = []() constexpr {
            std::array<std::array<Bitboard, Squares::count()>, Squares::count()> t{};

            auto addLine = [](Square s1, Square s2) constexpr {
                Bitboard b{};
                int df = s2.file() - s1.file();
                int dr = s2.rank() - s1.rank();
                
                if (df != 0) df /= (df < 0 ? -df : df);
                if (dr != 0) dr /= (dr < 0 ? -dr : dr);

                File f = s1.file();
                Rank r = s1.rank();
                while (true) {
                    b |= Bitboard::square(Square(f, r));
                    if (f == s2.file() && r == s2.rank()) break;
                    f += df;
                    r += dr;
                }
                return b;
            };

            for (Square s1 : Squares::all()) {
                for (Square s2 : Squares::all()) {
                    if (s1 != s2) {
                        if (s1.file() == s2.file() || s1.rank() == s2.rank() ||
                            s1.diag() == s2.diag() || s1.antiDiag() == s2.antiDiag()) {
                            t[s1.value()][s2.value()] = addLine(s1, s2);
                        }
                    }
                }
            }

            return t;
        }();
        return table[from.value()][to.value()];
    }

    static constexpr Bitboard between(Square from, Square to) {
        static constexpr auto table = []() constexpr {
            std::array<std::array<Bitboard, Squares::count()>, Squares::count()> t{};

            auto addLine = [](Square s1, Square s2) constexpr {
                Bitboard b{};
                int df = s2.file() - s1.file();
                int dr = s2.rank() - s1.rank();

                if (df != 0) df /= (df < 0 ? -df : df);
                if (dr != 0) dr /= (dr < 0 ? -dr : dr);

                File f = s1.file() + df;
                Rank r = s1.rank() + dr;

                while (f != s2.file() || r != s2.rank()) {
                    b |= Bitboard::square(Square(f, r));
                    f += df;
                    r += dr;
                }
                return b;
            };

            for (Square s1 : Squares::all()) {
                for (Square s2 : Squares::all()) {
                    if (s1 != s2) {
                        if (s1.file() == s2.file() || s1.rank() == s2.rank() ||
                            s1.diag() == s2.diag() || s1.antiDiag() == s2.antiDiag()) {
                            t[s1.value()][s2.value()] = addLine(s1, s2);
                        }
                    }
                }
            }

            return t;
        }();
        return table[from.value()][to.value()];
    }
};

namespace Bitboards {
inline constexpr Bitboard ZERO{0};
inline constexpr Bitboard ONE{1};
};  // namespace Bitboards