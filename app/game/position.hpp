#pragma once

#include <string>
#include <vector>

#include "bitboard.hpp"
#include "castling.hpp"
#include "en_passant.hpp"
#include "halfmove.hpp"
#include "move.hpp"
#include "navigation.hpp"
#include "piece.hpp"
#include "state.hpp"

constexpr auto DEFAULT_FEN       = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
const int      MAX_DEPTH         = 64;  // Hard cap for recursion
const int      MAX_MOVES_PER_POS = 256;

class Position {
   public:
    Position(const std::string &fen = DEFAULT_FEN) { setFromFEN(fen); }

    void                      setFromFEN(const std::string &fen = DEFAULT_FEN);
    [[nodiscard]] std::string toFEN() const;

    void doMove(Move move);
    void undoMove();

    [[nodiscard]] Color    turn() const { return m_turn; }
    [[nodiscard]] Castling castling() const { return m_castling; }
    [[nodiscard]] Piece    at(Square square) const { return m_board[square]; };

    [[nodiscard]] bool isLegal() const;

    [[nodiscard]] Bitboard occupancy() const { return m_color_bb[Color::WHITE] | m_color_bb[Color::BLACK]; }

    [[nodiscard]] Bitboard ourOccupancy() const { return m_color_bb[m_turn]; }
    [[nodiscard]] Bitboard ourOccupancy(PieceType piece_type) const {
        return m_color_bb[m_turn] & m_piece_type_bb[piece_type];
    }

    [[nodiscard]] Bitboard theirOccupancy() const { return m_color_bb[m_turn.flipped()]; }
    [[nodiscard]] Bitboard theirOccupancy(PieceType piece_type) const {
        return m_color_bb[m_turn.flipped()] & m_piece_type_bb[piece_type];
    }  // these better be some templates <SIDE (us/them/both)>

   private:
    void clear();
    void set(Square square, Piece p);
    void unset(Square square);

    void nextTurn();

    std::array<Piece, Square::number()>       m_board{};
    std::array<Bitboard, Color::number()>     m_color_bb{Bitboard::zero()};
    std::array<Bitboard, PieceType::number()> m_piece_type_bb{Bitboard::zero()};

    std::vector<State> m_states;
    std::vector<Move>  m_moves;

    Color     m_turn     = Color::WHITE;
    Castling  m_castling = Castling::ANY;
    EnPassant m_en_passant;
    Halfmove  m_halfmove;
};