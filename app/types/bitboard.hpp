
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
                File     file = _square.file();
                Rank     rank = _square.rank();

                for (auto _rank : Ranks::all()) {
                    for (auto _file : Files::all()) {
                        Square possible_square = Square(_file, _rank);
                        if (_file.value() - _rank.value() == file.value() - rank.value()) {
                            diag = diag | Bitboard::square(possible_square);
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
                File     file      = _square.file();
                Rank     rank      = _square.rank();

                for (auto _rank : Ranks::all()) {
                    for (auto _file : Files::all()) {
                        Square possible_square = Square(_file, _rank);
                        if (_file.value() + _rank.value() == file.value() + rank.value()) {
                            anti_diag = anti_diag | Bitboard::square(possible_square);
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

        for (auto square : Squares::all()) {
            Bitboard pseudo_attacks = Bitboard(0);
            for (const auto& direction : directions) {
                Square destination = square.move(direction);
                if (destination != Squares::NONE) pseudo_attacks |= Bitboard::square(destination);
            }
            table[square.value()] = pseudo_attacks;
        }

        return table;
    }

    static constexpr Bitboard slidingAttacks(Square from, std::span<const Direction> directions, Bitboard occupancy) {
        Bitboard attacks{};
        for (Direction direction : directions) {
            Square square = from;
            while ((square = square.move(direction)) != Squares::NONE) {
                auto bitboard = Bitboard::square(square);
                attacks |= bitboard;
                if ((occupancy & bitboard).any()) break;
            }
        }
        return attacks;
    }

    static constexpr Bitboard slidingAttacks(Square from, Direction direction, Bitboard occupancy) {
        Bitboard attacks{};

        Square square = from;
        while ((square = square.move(direction)) != Squares::NONE) {
            auto bitboard = Bitboard::square(square);
            attacks |= bitboard;
            if ((occupancy & bitboard).any()) break;
        }

        return attacks;
    }

    static constexpr Bitboard between(Square from, Square to) {
        static constexpr auto table = []() constexpr {
            std::array<std::array<Bitboard, Squares::count()>, Squares::count()> _table{};

            for (auto square1 : Squares::all()) {
                for (auto square2 : Squares::all()) {
                    if (square1.value() >= square2.value()) continue;
                    if (Square::distance(square1, square2) <= 1) continue;

                    Bitboard between;

                    int dr = square2.rank().value() - square1.rank().value();
                    int df = square2.file().value() - square1.file().value();

                    if (square1.rank() == square2.rank()) {
                        between =
                            slidingAttacks(square1, dr < 0 ? Directions::W : Directions::E, Bitboard::square(square2)) &
                            slidingAttacks(square2, dr < 0 ? Directions::E : Directions::W, Bitboard::square(square1));
                    } else if (square1.file() == square2.file()) {
                        between =
                            slidingAttacks(square1, df < 0 ? Directions::S : Directions::N, Bitboard::square(square2)) &
                            slidingAttacks(square2, df < 0 ? Directions::N : Directions::S, Bitboard::square(square1));
                    } else if (dr == df) {
                        between = slidingAttacks(square1, dr < 0 ? Directions::SW : Directions::NE,
                                                 Bitboard::square(square2)) &
                                  slidingAttacks(square2, dr < 0 ? Directions::NE : Directions::SW,
                                                 Bitboard::square(square1));
                    } else if (dr == -df) {
                        between = slidingAttacks(square1, dr < 0 ? Directions::NW : Directions::SE,
                                                 Bitboard::square(square2)) &
                                  slidingAttacks(square2, dr < 0 ? Directions::SE : Directions::NW,
                                                 Bitboard::square(square1));
                    }

                    _table[square1.value()][square2.value()] = between;
                    _table[square2.value()][square1.value()] = between;
                }
            }

            return _table;
        }();

        return table[from.value()][to.value()];
    }
};

namespace Bitboards {
inline constexpr Bitboard ZERO{0};
inline constexpr Bitboard ONE{1};
};  // namespace Bitboards