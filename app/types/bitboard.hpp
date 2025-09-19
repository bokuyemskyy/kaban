
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

            for (auto rank : Ranks::all()) {
                t[rank.value()] = (RANK_A << (rank.value() * Files::count()));
            }
            return t;
        }();

        return table[rank.value()];
    }

    static constexpr Bitboard file(File file) {
        static constexpr auto table = []() constexpr {
            std::array<Bitboard, Files::count()> t{};

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
            std::array<Bitboard, Squares::count()> t{};

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
            std::array<Bitboard, Squares::count()> t{};
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
        for (Direction dir : directions) {
            Square sq = from;
            while ((sq = sq.move(dir)) != Squares::NONE) {
                auto bb = Bitboard::square(sq);
                attacks |= bb;
                if ((occupancy & bb).any()) break;
            }
        }
        return attacks;
    }

    static constexpr Bitboard slidingAttacks(Square from, Direction direction, Bitboard occupancy) {
        Bitboard attacks{};

        Square square = from;
        while ((square = square.move(direction)) != Squares::NONE) {
            auto square_bb = Bitboard::square(square);
            attacks |= square_bb;
            if ((occupancy & square_bb).any()) break;
        }

        return attacks;
    }

    static constexpr Bitboard between(Square from, Square to) {
        static constexpr auto table = []() constexpr {
            std::array<std::array<Bitboard, Squares::count()>, Squares::count()> t{};

            for (auto sq1 : Squares::all()) {
                for (auto sq2 : Squares::all()) {
                    if (sq1.value() >= sq2.value()) continue;
                    if (Square::distance(sq1, sq2) <= 1) continue;

                    Bitboard between;

                    int dr = sq2.rank().value() - sq1.rank().value();
                    int df = sq2.file().value() - sq1.file().value();

                    if (sq1.rank() == sq2.rank()) {
                        between = slidingAttacks(sq1, dr < 0 ? Directions::W : Directions::E, Bitboard::square(sq2)) &
                                  slidingAttacks(sq2, dr < 0 ? Directions::E : Directions::W, Bitboard::square(sq1));
                    } else if (sq1.file() == sq2.file()) {
                        between = slidingAttacks(sq1, df < 0 ? Directions::S : Directions::N, Bitboard::square(sq2)) &
                                  slidingAttacks(sq2, df < 0 ? Directions::N : Directions::S, Bitboard::square(sq1));
                    } else if (dr == df) {
                        between = slidingAttacks(sq1, dr < 0 ? Directions::SW : Directions::NE, Bitboard::square(sq2)) &
                                  slidingAttacks(sq2, dr < 0 ? Directions::NE : Directions::SW, Bitboard::square(sq1));
                    } else if (dr == -df) {
                        between = slidingAttacks(sq1, dr < 0 ? Directions::NW : Directions::SE, Bitboard::square(sq2)) &
                                  slidingAttacks(sq2, dr < 0 ? Directions::SE : Directions::NW, Bitboard::square(sq1));
                    }

                    t[sq1.value()][sq2.value()] = between;
                    t[sq2.value()][sq1.value()] = between;
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