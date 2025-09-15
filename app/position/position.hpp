#pragma once

#include <string>
#include <vector>

#include "bit_operations.hpp"
#include "bitboard.hpp"
#include "castling.hpp"
#include "color.hpp"
#include "direction.hpp"
#include "en_passant.hpp"
#include "halfmove.hpp"
#include "magic.hpp"
#include "move.hpp"
#include "piece.hpp"
#include "piece_type.hpp"
#include "square.hpp"
#include "undo_info.hpp"

enum class Side : uint8_t {
    US,
    THEM,
    BOTH
};

enum class GenerationType : uint8_t {
    ALL,
    LEGAL
};

class Position {
   public:
    Position(const std::string& fen = DEFAULT_FEN) { fromFen(fen); }

    void fromFen(const std::string& fen = DEFAULT_FEN);
    void clear() { fromFen(""); };

    [[nodiscard]] std::string toFen() const;

    UndoInfo makeMove(Move move);
    void     unmakeMove(Move move, UndoInfo undo_info);

    [[nodiscard]] auto us() const { return m_stm; }
    [[nodiscard]] auto castling() const { return m_castling; }

    [[nodiscard]] auto board() const { return m_board; }
    [[nodiscard]] auto at(Square square) const { return m_board[square.value()]; };

    [[nodiscard]] bool isLegal() const;

    template <GenerationType GT>
    std::vector<Move> generateMoves() const {
        std::vector<Move> moves{};

        if (m_stm == Colors::WHITE)
            generatePawnMoves<Colors::WHITE>(occupancy<Side::US>(PieceTypes::PAWN), moves);
        else
            generatePawnMoves<Colors::BLACK>(occupancy<Side::US>(PieceTypes::PAWN), moves);

        generatePieceMoves<PieceTypes::KNIGHT>(occupancy<Side::US>(PieceTypes::KNIGHT), moves);
        generatePieceMoves<PieceTypes::BISHOP>(occupancy<Side::US>(PieceTypes::BISHOP), moves);
        generatePieceMoves<PieceTypes::ROOK>(occupancy<Side::US>(PieceTypes::ROOK), moves);
        generatePieceMoves<PieceTypes::QUEEN>(occupancy<Side::US>(PieceTypes::QUEEN), moves);
        generatePieceMoves<PieceTypes::KING>(occupancy<Side::US>(PieceTypes::KING), moves);

        return moves;
    }

    template <GenerationType GT>
    std::vector<Move> generateMoves(Square square) const {
        std::vector<Move> moves{};
        auto              piece = at(square);
        if (!piece.hasValue() || piece.color() != m_stm) return moves;

        auto piece_type = piece.pieceType();
        auto piece_mask = Bitboard::square(square);

        if (piece_type == PieceTypes::PAWN) {
            if (m_stm == Colors::WHITE)
                generatePawnMoves<Colors::WHITE>(occupancy<Side::US>(PieceTypes::PAWN) & piece_mask, moves);
            else
                generatePawnMoves<Colors::BLACK>(occupancy<Side::US>(PieceTypes::PAWN) & piece_mask, moves);
        } else if (piece_type == PieceTypes::KNIGHT) {
            generatePieceMoves<PieceTypes::KNIGHT>(occupancy<Side::US>(PieceTypes::KNIGHT) & piece_mask, moves);
        } else if (piece_type == PieceTypes::BISHOP) {
            generatePieceMoves<PieceTypes::BISHOP>(occupancy<Side::US>(PieceTypes::BISHOP) & piece_mask, moves);
        } else if (piece_type == PieceTypes::ROOK) {
            generatePieceMoves<PieceTypes::ROOK>(occupancy<Side::US>(PieceTypes::ROOK) & piece_mask, moves);
        } else if (piece_type == PieceTypes::QUEEN) {
            generatePieceMoves<PieceTypes::QUEEN>(occupancy<Side::US>(PieceTypes::QUEEN) & piece_mask, moves);
        } else if (piece_type == PieceTypes::KING) {
            generatePieceMoves<PieceTypes::KING>(occupancy<Side::US>(PieceTypes::KING) & piece_mask, moves);
        }
        return moves;
    }

    template <Side S>
    [[nodiscard]] Bitboard occupancy() const {
        if constexpr (S == Side::US)
            return m_color[m_stm.value()];
        else if constexpr (S == Side::THEM)
            return m_color[(!m_stm).value()];
        else
            return m_color[Colors::WHITE.value()] | m_color[Colors::BLACK.value()];
    }

    template <Side S>
    [[nodiscard]] Bitboard occupancy(PieceType pt) const {
        return occupancy<S>() & m_pieceType[pt.value()];
    }

   private:
    void set(Square square, Piece p);
    void unset(Square square);

    std::array<Piece, Square::count()>       m_board{};
    std::array<Bitboard, Color::count()>     m_color{Bitboards::ZERO};
    std::array<Bitboard, PieceType::count()> m_pieceType{Bitboards::ZERO};

    Color     m_stm      = Colors::WHITE;
    Castling  m_castling = Castlings::ANY;
    EnPassant m_en_passant;
    Halfmove  m_halfmove;

    Magics m_magics;

    template <PieceType PT>
    [[nodiscard]] constexpr Bitboard pseudoAttacks(Square square) const {
        if constexpr (PT == PieceTypes::KNIGHT) {
            static constexpr auto table = Bitboard::pseudoAttacks(PieceTypes::KNIGHT.directions());
            return table[square.value()];
        } else if constexpr (PT == PieceTypes::KING) {
            static constexpr auto table = Bitboard::pseudoAttacks(PieceTypes::KING.directions());
            return table[square.value()];
        } else if constexpr (PT == PieceTypes::BISHOP) {
            return m_magics.lookup<PieceTypes::BISHOP>(square, occupancy<Side::BOTH>());
        } else if constexpr (PT == PieceTypes::ROOK) {
            return m_magics.lookup<PieceTypes::ROOK>(square, occupancy<Side::BOTH>());
        } else if constexpr (PT == PieceTypes::QUEEN) {
            return m_magics.lookup<PieceTypes::ROOK>(square, occupancy<Side::BOTH>()) |
                   m_magics.lookup<PieceTypes::BISHOP>(square, occupancy<Side::BOTH>());
        }
    }

    template <Color C>
    [[nodiscard]] constexpr Bitboard pawnAttacks(Square square) const {
        if constexpr (C == Colors::WHITE) {
            static constexpr auto table = Bitboard::pseudoAttacks(PieceTypes::PAWN.directions(Colors::WHITE));
            return table[square.value()];
        } else {
            static constexpr auto table = Bitboard::pseudoAttacks(PieceTypes::PAWN.directions(Colors::BLACK));
            return table[square.value()];
        }
    }

    template <Color C>
    void generatePawnMoves(Bitboard pieces, std::vector<Move>& moves) const {
        while (pieces.hasValue()) {
            Square from = poplsb(pieces);

            Bitboard attacks = pawnAttacks<C>(from) & occupancy<Side::THEM>();
            while (attacks.hasValue()) moves.emplace_back(Move(from, poplsb(attacks)));

            Bitboard push;
            if constexpr (C == Colors::WHITE) {
                push = Bitboard::square(from.shifted(Directions::N)) & ~occupancy<Side::BOTH>();
                if ((from.rank() == Ranks::R2) && push.hasValue()) {
                    push |= push << Directions::N.value();
                }
            } else {
                push = Bitboard::square(from.shifted(Directions::S)) & ~occupancy<Side::BOTH>();
                if ((from.rank() == Ranks::R7) && push.hasValue()) {
                    push |= push >> -Directions::S.value();
                }
            }
            while (push.hasValue()) moves.emplace_back(Move(from, poplsb(push)));
        }
    }

    template <PieceType PT>
    void generatePieceMoves(Bitboard pieces, std::vector<Move>& moves) const {
        while (pieces.hasValue()) {
            Square   from    = poplsb(pieces);
            Bitboard targets = pseudoAttacks<PT>(from) & ~occupancy<Side::US>();
            while (targets.hasValue()) {
                moves.emplace_back(Move(from, poplsb(targets)));
            }
        }
    }

    static constexpr auto DEFAULT_FEN       = "rnbqkbnr/pppppppp/8/8/8/5p2/1P3PPP/RNBQKBNR w";
    static constexpr int  MAX_DEPTH         = 64;
    static constexpr int  MAX_MOVES_PER_POS = 256;
};