#pragma once

#include <string>
#include <vector>

#include "bitboard.hpp"
#include "castling.hpp"
#include "move.hpp"
#include "navigation.hpp"
#include "piece.hpp"

constexpr auto DEFAULT_FEN       = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
const int      MAX_DEPTH         = 64;  // Hard cap for recursion
const int      MAX_MOVES_PER_POS = 256;

class Position {
   public:
    Position(const std::string &fen = DEFAULT_FEN) { setFromFEN(fen); }

    void                      setFromFEN(const std::string &fen = DEFAULT_FEN);
    [[nodiscard]] std::string toFEN() const;

    [[nodiscard]] Piece at(Square square) const { return m_board[square]; };

    [[nodiscard]] bool isLegal();

    void doMove(Move move);
    void undoMove();

    [[nodiscard]] Color    turn() const { return m_turn; }
    [[nodiscard]] bool     isLegal() const;
    [[nodiscard]] Castling castlingRights() const { return m_castling; }

   private:
    void clear();
    void set(Square square, Piece p);
    void unset(Square square);

    void nextTurn();

    std::array<Piece, Square::NB>       m_board{};
    std::array<Bitboard, Color::NB>     m_colorBB{Bitboard::zero()};
    std::array<Bitboard, PieceType::NB> m_pieceTypeBB{Bitboard::zero()};

    std::vector<State> m_states{};
    std::vector<Move>  m_moves;

    Color    m_turn      = Color::WHITE;
    Castling m_castling  = Castling::ANY;
    uint8_t  m_halfmoves = 0;
};