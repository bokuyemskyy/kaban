#ifndef POSITION_HPP
#define POSITION_HPP

#include <array>
#include <cstddef>
#include <list>
#include <string>
#include <vector>

#include "bitboard.hpp"
#include "move.hpp"
#include "piece.hpp"
#include "square.hpp"
#include "utils.hpp"

constexpr auto DEFAULT_FEN       = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
const int      MAX_DEPTH         = 64;  // Hard cap for recursion
const int      MAX_MOVES_PER_POS = 256;

class Position {
   public:
    Position(const std::string &fen = DEFAULT_FEN) { setFromFEN(fen); }

    void                      setFromFEN(const std::string &fen);
    [[nodiscard]] std::string toFEN() const;

    void setPiece(Square square, Piece p);
    void unsetPiece(Square square);
    void resetBoard();

    [[nodiscard]] Piece pieceAt(Square square) const { return m_board[square]; };

    void toggleTurn();

    [[nodiscard]] bool isLegal();

    void makeMoveUci(std::string moveUci) {
        Move move = createMove(stringToSquare(moveUci.substr(0, 2)), stringToSquare(moveUci.substr(2, 2)), 0);
        doMove(move);
    }

    void doMove(Move move);
    void undoMove();

    [[nodiscard]] Color turn() const { return m_turn; }

   private:
    std::array<Piece, Squares::NB>       m_board{};
    std::array<Bitboard, Colors::NB>     m_colorBB{Bitboards::ZERO};
    std::array<Bitboard, PieceTypes::NB> m_pieceTypeBB{Bitboards::ZERO};

    std::list<Delta> m_deltas;
    std::list<Move>  m_moves;

    Color    m_turn      = Colors::WHITE;
    Castling m_castling  = Castlings::ANY;
    uint8_t  m_halfmoves = 0;
};

#endif