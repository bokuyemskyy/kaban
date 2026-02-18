#pragma once

#include <array>
#include <cassert>
#include <chrono>
#include <cmath>
#include <iostream>
#include <ostream>
#include <string>

#include "bit_operations.hpp"
#include "bitboard.hpp"
#include "castling.hpp"
#include "color.hpp"
#include "direction.hpp"
#include "en_passant.hpp"
#include "halfmove.hpp"
#include "magics.hpp"
#include "move.hpp"
#include "move_flag.hpp"
#include "piece.hpp"
#include "piece_type.hpp"
#include "square.hpp"
#include "undo_info.hpp"

enum class Sides : uint8_t {
    US,
    THEM,
    BOTH
};

enum class GenerationTypes : uint8_t {
    ALL,
    LEGAL
};

using Board = std::array<Piece, Squares::count()>;

class Position {
   public:
    explicit Position(const std::string& fen = DEFAULT_FEN) { fromFen(fen); }

    void fromFen(const std::string& fen = DEFAULT_FEN);
    void reset();

    [[nodiscard]] std::string toFen() const;

    [[nodiscard]] bool isCheck() const {
        Square king = lsb(occupancy(m_stm, PieceTypes::KING));
        return isAttacked(king, !m_stm);
    }

    UndoInfo makeMove(Move move);
    void     unmakeMove(Move move, const UndoInfo& undo_info);

    [[nodiscard]] auto us() const { return m_stm; }
    [[nodiscard]] auto castling() const { return m_castling; }

    [[nodiscard]] const auto& board() const { return m_board; }

    Piece&                        at(Square square) { return m_board[square.value()]; }
    [[nodiscard]] const Piece&    at(Square square) const { return m_board[square.value()]; }
    Bitboard&                     at(Color color) { return m_color[color.value()]; }
    [[nodiscard]] const Bitboard& at(Color color) const { return m_color[color.value()]; }
    Bitboard&                     at(PieceType piece_type) { return m_piece_type[piece_type.value()]; }
    [[nodiscard]] const Bitboard& at(PieceType piece_type) const { return m_piece_type[piece_type.value()]; }

    template <bool F>
    [[nodiscard]] bool isLegal() const {
        if constexpr (F) {  // first check, when new position is set. the moves are guaranteed to be legal
            if (popcount(occupancy(m_stm, PieceTypes::KING)) != 1) return false;
            if (popcount(occupancy(!m_stm, PieceTypes::KING)) != 1) return false;
            if (popcount(occupancy(m_stm, PieceTypes::PAWN)) > 8) return false;
            if (popcount(occupancy(!m_stm, PieceTypes::PAWN)) > 8) return false;
            if (popcount(occupancy(m_stm)) > 16) return false;
            if (popcount(occupancy(!m_stm)) > 16) return false;
            if ((occupancy(PieceTypes::PAWN) & (Bitboard::rank(Ranks::R1) | Bitboard::rank(Ranks::R8))).any())
                return false;
        }

        Square king = lsb(occupancy(!m_stm, PieceTypes::KING));

        return !isAttacked(king, m_stm);
    }

    [[nodiscard]] bool isAttacked(Square square, Color attacker) const;

    template <GenerationTypes GT>
    size_t generateMoves(Move* move_list) {
        Move* first = move_list;
        if constexpr (GT == GenerationTypes::ALL) {
            if (m_stm == Colors::WHITE)
                move_list = generatePawnMoves<Colors::WHITE>(occupancy(m_stm, PieceTypes::PAWN), move_list);
            else
                move_list = generatePawnMoves<Colors::BLACK>(occupancy(m_stm, PieceTypes::PAWN), move_list);

            move_list = generatePieceMoves<PieceTypes::KNIGHT>(occupancy(m_stm, PieceTypes::KNIGHT), move_list);
            move_list = generatePieceMoves<PieceTypes::BISHOP>(occupancy(m_stm, PieceTypes::BISHOP), move_list);
            move_list = generatePieceMoves<PieceTypes::ROOK>(occupancy(m_stm, PieceTypes::ROOK), move_list);
            move_list = generatePieceMoves<PieceTypes::QUEEN>(occupancy(m_stm, PieceTypes::QUEEN), move_list);
            move_list = generatePieceMoves<PieceTypes::KING>(occupancy(m_stm, PieceTypes::KING), move_list);

            move_list = generateCastling(move_list);
            return static_cast<size_t>(move_list - first);
        } else if constexpr (GT == GenerationTypes::LEGAL) {
            Move*       start = move_list;
            Move const* end   = start + generateMoves<GenerationTypes::ALL>(move_list);

            Move* out = start;

            for (Move const* m = start; m < end; ++m) {
                UndoInfo undo = makeMove(*m);
                if (isLegal<false>()) *out++ = *m;
                unmakeMove(*m, undo);
            }

            return static_cast<size_t>(out - start);
        } else {
            return static_cast<size_t>(move_list - first);
        }
    }

    [[nodiscard]] Bitboard occupancy(Color c) const { return m_color[c.value()]; }
    [[nodiscard]] Bitboard occupancy(Color c, PieceType pt) const {
        return m_color[c.value()] & m_piece_type[pt.value()];
    }
    [[nodiscard]] Bitboard occupancy(PieceType pt) const { return m_piece_type[pt.value()]; }
    [[nodiscard]] Bitboard occupancyAll() const {
        return m_color[Colors::WHITE.value()] | m_color[Colors::BLACK.value()];
    }

   private:
    void setPiece(Square square, Piece p);
    void unsetPiece(Square square);
    void movePiece(Square from, Square to);

    Board                                     m_board{};
    std::array<Bitboard, Colors::count()>     m_color{Bitboards::ZERO};
    std::array<Bitboard, PieceTypes::count()> m_piece_type{Bitboards::ZERO};

    Color     m_stm      = Colors::WHITE;
    Castling  m_castling = Castlings::ANY;
    EnPassant m_en_passant{};
    Halfmove  m_halfmove{};

    template <PieceType PT>
    [[nodiscard]] constexpr Bitboard pseudoAttacks(Square square) const {
        if constexpr (PT == PieceTypes::KNIGHT) {
            static constexpr auto table = Bitboard::pseudoAttacks(Directions::of(PieceTypes::KNIGHT));
            return table[square.value()];
        } else if constexpr (PT == PieceTypes::KING) {
            static constexpr auto table = Bitboard::pseudoAttacks(Directions::of(PieceTypes::KING));
            return table[square.value()];
        } else if constexpr (PT == PieceTypes::BISHOP) {
            return Magics::get().lookup<PieceTypes::BISHOP>(square, occupancyAll());
        } else if constexpr (PT == PieceTypes::ROOK) {
            return Magics::get().lookup<PieceTypes::ROOK>(square, occupancyAll());
        } else if constexpr (PT == PieceTypes::QUEEN) {
            return Magics::get().lookup<PieceTypes::ROOK>(square, occupancyAll()) |
                   Magics::get().lookup<PieceTypes::BISHOP>(square, occupancyAll());
        }
    }

    template <Color C>
    [[nodiscard]] static constexpr Bitboard pawnAttacks(Square square) {
        if constexpr (C == Colors::WHITE) {
            static constexpr auto table = Bitboard::pseudoAttacks(Directions::of(Pieces::W_PAWN));
            return table[square.value()];
        } else {
            static constexpr auto table = Bitboard::pseudoAttacks(Directions::of(Pieces::B_PAWN));
            return table[square.value()];
        }
    }

    template <Color C>
    Move* generatePawnMoves(Bitboard pieces, Move* move_list) const {
        while (pieces.any()) {
            Square from = poplsb(pieces);

            Bitboard attacks = pawnAttacks<C>(from) & occupancy(!m_stm);
            while (attacks.any()) {
                Square to = poplsb(attacks);
                if (to.rank() == (C == Colors::WHITE ? Ranks::R8 : Ranks::R1)) {
                    *move_list++ = Move(from, to, MoveFlags::PROMOTION_QUEEN);
                    *move_list++ = Move(from, to, MoveFlags::PROMOTION_ROOK);
                    *move_list++ = Move(from, to, MoveFlags::PROMOTION_BISHOP);
                    *move_list++ = Move(from, to, MoveFlags::PROMOTION_KNIGHT);
                } else {
                    *move_list++ = Move(from, to);
                }
            }

            if (m_en_passant.hasValue()) {
                if constexpr (C == Colors::WHITE) {
                    if ((pawnAttacks<C>(from) & Bitboard::square(Square(m_en_passant.file(), Ranks::R6))).any())
                        *move_list++ = Move(from, Square(m_en_passant.file(), Ranks::R6), MoveFlags::EN_PASSANT);
                } else {
                    if ((pawnAttacks<C>(from) & Bitboard::square(Square(m_en_passant.file(), Ranks::R3))).any()) {
                        *move_list++ = Move(from, Square(m_en_passant.file(), Ranks::R3), MoveFlags::EN_PASSANT);
                    }
                }
            }

            Square single_push = from.move(C == Colors::WHITE ? Directions::N : Directions::S);
            if ((Bitboard::square(single_push) & ~occupancyAll()).any()) {
                if (single_push.rank() == (C == Colors::WHITE ? Ranks::R8 : Ranks::R1)) {
                    *move_list++ = Move(from, single_push, MoveFlags::PROMOTION_QUEEN);
                    *move_list++ = Move(from, single_push, MoveFlags::PROMOTION_ROOK);
                    *move_list++ = Move(from, single_push, MoveFlags::PROMOTION_BISHOP);
                    *move_list++ = Move(from, single_push, MoveFlags::PROMOTION_KNIGHT);
                } else {
                    *move_list++ = Move(from, single_push);

                    if constexpr (C == Colors::WHITE) {
                        if (from.rank() == Ranks::R2) {
                            Square double_push = single_push.move(Directions::N);
                            if ((Bitboard::square(double_push) & ~occupancyAll()).any())
                                *move_list++ = Move(from, double_push, MoveFlags::PAWN_DOUBLE_PUSH);
                        }
                    } else {
                        if (from.rank() == Ranks::R7) {
                            Square double_push = single_push.move(Directions::S);
                            if ((Bitboard::square(double_push) & ~occupancyAll()).any())
                                *move_list++ = Move(from, double_push, MoveFlags::PAWN_DOUBLE_PUSH);
                        }
                    }
                }
            }
        }
        return move_list;
    }

    template <PieceType PT>
    Move* generatePieceMoves(Bitboard pieces, Move* move_list) const {
        while (pieces.any()) {
            Square   from    = poplsb(pieces);
            Bitboard targets = pseudoAttacks<PT>(from) & ~occupancy(m_stm);
            while (targets.any()) {
                *move_list++ = Move(from, poplsb(targets));
            }
        }
        return move_list;
    }

    Move* generateCastling(Move* move_list) const {
        if (m_stm == Colors::WHITE) {
            if (m_castling.has(Castlings::W_KING_SIDE) &&
                !(Bitboard::between(Squares::E1, Squares::H1) & occupancyAll()).any()) {
                if (!isAttacked(Squares::E1, Colors::BLACK) && !isAttacked(Squares::F1, Colors::BLACK))
                    *move_list++ = Move(Squares::E1, Squares::G1, MoveFlags::CASTLING_KING);
            }
            if (m_castling.has(Castlings::W_QUEEN_SIDE) &&
                !(Bitboard::between(Squares::E1, Squares::A1) & occupancyAll()).any()) {
                if (!isAttacked(Squares::E1, Colors::BLACK) && !isAttacked(Squares::D1, Colors::BLACK))
                    *move_list++ = Move(Squares::E1, Squares::C1, MoveFlags::CASTLING_QUEEN);
            }
        } else {
            if (m_castling.has(Castlings::B_KING_SIDE) &&
                !(Bitboard::between(Squares::E8, Squares::H8) & occupancyAll()).any()) {
                if (!isAttacked(Squares::E8, Colors::WHITE) && !isAttacked(Squares::F8, Colors::WHITE))
                    *move_list++ = Move(Squares::E8, Squares::G8, MoveFlags::CASTLING_KING);
            }
            if (m_castling.has(Castlings::B_QUEEN_SIDE) &&
                !(Bitboard::between(Squares::E8, Squares::A8) & occupancyAll()).any()) {
                if (!isAttacked(Squares::E8, Colors::WHITE) && !isAttacked(Squares::D8, Colors::WHITE))
                    *move_list++ = Move(Squares::E8, Squares::C8, MoveFlags::CASTLING_QUEEN);
            }
        }
        return move_list;
    }

    static constexpr auto DEFAULT_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w";
};