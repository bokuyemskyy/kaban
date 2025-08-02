#ifndef POSITION_HPP
#define POSITION_HPP

#include <array>
#include <list>
#include <string>

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

    void setPiece(Square square, Piece p);
    void unsetPiece(Square square);
    void resetBoard();

    [[nodiscard]] Piece pieceAt(Square square) const { return m_board[square]; };

    void toggleTurn();

    [[nodiscard]] bool isLegal();

    void makeMoveUci(std::string moveUci) {
        Move move = Move(Square(moveUci.substr(0, 2)), Square(moveUci.substr(2, 2)), 0, 0);
        doMove(move);
    }

    void doMove(Move move);
    void undoMove();

    [[nodiscard]] Color turn() const { return m_turn; }

   private:
    std::array<Piece, Square::NB>       m_board{};
    std::array<Bitboard, Color::NB>     m_colorBB{Bitboard::zero()};
    std::array<Bitboard, PieceType::NB> m_pieceTypeBB{Bitboard::zero()};

    // std::list<Delta> m_deltas;
    std::list<Move> m_moves;

    Color    m_turn      = Color::WHITE;
    Castling m_castling  = Castling::ANY;
    uint8_t  m_halfmoves = 0;
};

#endif