#ifndef ATTACKS_HPP
#define ATTACKS_HPP

#include <array>

#include "bitboard.hpp"
#include "magic.hpp"
#include "navigation.hpp"
#include "piece.hpp"

class AttackGenerator {
   public:
    static constexpr Bitboard pawnAttacksBB(Color color, Square square) {
        static constexpr auto table = []() constexpr {
            std::array<std::array<Bitboard, Square::NB>, Color::NB> t{};
            for (auto square : Square::all()) {
                t[Color::WHITE][square] =
                    Bitboard::destinationBB(square, Direction::NW) | Bitboard::destinationBB(square, Direction::NE);
                t[Color::BLACK][square] =
                    Bitboard::destinationBB(square, Direction::SW) | Bitboard::destinationBB(square, Direction::SE);
            }
            return t;
        }();
        return table[color][square];
    }

    static constexpr Bitboard knightAttacksBB(Square square) {
        static constexpr auto table = []() constexpr {
            std::array<Bitboard, Square::NB> t{};

            constexpr std::array<Direction, 8> knightDirections = {Direction::NNE, Direction::NNW, Direction::ENE,
                                                                   Direction::WNW, Direction::SSE, Direction::SSW,
                                                                   Direction::ESE, Direction::WSW};

            for (auto square : Square::all()) {
                Bitboard attacks = Bitboard::zero();
                for (auto direction : knightDirections) {
                    attacks |= Bitboard::destinationBB(square, direction);
                }
                t[square] = attacks;
            }
            return t;
        }();
        return table[square];
    }

    static constexpr Bitboard kingAttacksBB(Square square) {
        static constexpr auto table = []() constexpr {
            std::array<Bitboard, Square::NB> t{};

            constexpr std::array<Direction, 8> kingDirections = {Direction::E,  Direction::N,  Direction::W,
                                                                 Direction::S,  Direction::NE, Direction::NW,
                                                                 Direction::SE, Direction::SW};

            for (auto square : Square::all()) {
                Bitboard attacks = Bitboard::zero();
                for (auto direction : kingDirections) {
                    attacks |= Bitboard::destinationBB(square, direction);
                }
                t[square] = attacks;
            }
            return t;
        }();
        return table[square];
    }

    template <PieceType PT>
    static constexpr Bitboard attacksBB(Square square, Bitboard occupied = Bitboard::zero()) {
        if constexpr (PT == PieceType::KNIGHT) {
            return knightAttacksBB(square);
        } else if constexpr (PT == PieceType::BISHOP) {
            return bishopAttacksBB(square, occupied);
        } else if constexpr (PT == PieceType::ROOK) {
            return rookAttacksBB(square, occupied);
        } else if constexpr (PT == PieceType::QUEEN) {
            return queenAttacksBB(square, occupied);
        } else if constexpr (PT == PieceType::KING) {
            return kingAttacksBB(square);
        } else {
            static_assert(PT != PieceType::PAWN, "Use pawnAttacksBB with color parameter for pawn attacks");
            return Bitboard::zero();
        }
    }

    template <PieceType PT>
    static constexpr Bitboard attacksBB(Color color, Square square, Bitboard occupied = Bitboard::zero()) {
        if constexpr (PT == PieceType::PAWN) {
            return pawnAttacksBB(color, square);
        } else {
            return attacksBB<PT>(square, occupied);
        }
    }

    static constexpr Bitboard rookAttacks(Square square, Bitboard occupied) { return rookAttacksBB(square, occupied); }

    static constexpr Bitboard bishopAttacks(Square square, Bitboard occupied) {
        return bishopAttacksBB(square, occupied);
    }

    static constexpr Bitboard queenAttacks(Square square, Bitboard occupied) {
        return queenAttacksBB(square, occupied);
    }

    static constexpr Bitboard knightAttacks(Square square) { return knightAttacksBB(square); }

    static constexpr Bitboard kingAttacks(Square square) { return kingAttacksBB(square); }

    static constexpr Bitboard pawnAttacks(Color color, Square square) { return pawnAttacksBB(color, square); }

    template <PieceType PT>
    static constexpr bool isAttackedBy(Square target, Square attacker, Bitboard occupied = Bitboard::zero()) {
        return (attacksBB<PT>(attacker, occupied) & Bitboard::squareBB(target)) != Bitboard::zero();
    }

    template <PieceType PT>
    static constexpr bool isAttackedBy(Color color, Square target, Square attacker,
                                       Bitboard occupied = Bitboard::zero()) {
        if constexpr (PT == PieceType::PAWN) {
            return (pawnAttacksBB(color, attacker) & Bitboard::squareBB(target)) != Bitboard::zero();
        } else {
            return isAttackedBy<PT>(target, attacker, occupied);
        }
    }
};

#endif