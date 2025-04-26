#ifndef POSITION_HPP
#define POSITION_HPP

#include <array>
#include <list>
#include <string>
#include <vector>

#include "piece.hpp"
#include "precomputed.hpp"
#include "square.hpp"

constexpr auto DEFAULT_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";

class Position {
   public:
    Position(const std::string &fen = DEFAULT_FEN) {
        setFromFen(fen);
        precomputeAll();
    }

    void                      setFromFen(const std::string &fen);
    [[nodiscard]] std::string toFen() const;

    void                   setPiece(Square square, Piece p);
    void                   unsetPiece(Square square);
    void                   resetBoard();
    [[nodiscard]] Piece    pieceAt(Square square) const;
    [[nodiscard]] bool     isEmpty(Square square) const;
    [[nodiscard]] bool     isAlly(Square square) const;
    [[nodiscard]] Bitboard getAllyBb(Color us) const;
    [[nodiscard]] bool     isOpponent(Square square) const;
    [[nodiscard]] bool     isOpponent(Square square, PieceType pt) const;

    [[nodiscard]] bool isLegal();

    void generatePawnMoves(std::vector<Move> &moves, Square square) const;
    void generateKnightMoves(std::vector<Move> &moves, Square square) const;
    void generateBishopMoves(std::vector<Move> &moves, Square square) const;
    void generateRookMoves(std::vector<Move> &moves, Square square) const;
    void generateQueenMoves(std::vector<Move> &moves, Square square) const;
    void generateKingMoves(std::vector<Move> &moves, Square square) const;

    void generatePseudoLegalMoves(std::vector<Move> &moves);
    int  perft(uint8_t depth, bool verbose = false);

    void                doMove(Move move);
    void                undoMove();
    [[nodiscard]] Color turn() const { return m_turn; }

   private:
    std::array<Piece, Squares::NB>         m_board{};
    std::array<Bitboard, Colors::LAST + 1> m_colorBB{BITBOARD_ZERO};
    std::array<Bitboard, PieceTypes::NB>   m_pieceTypeBB{BITBOARD_ZERO};

    std::list<Delta> m_deltas;
    std::list<Move>  m_moves;

    Color    m_turn      = Colors::WHITE;
    Castling m_castling  = Castlings::ANY;
    uint8_t  m_halfmoves = 0;
};

#endif