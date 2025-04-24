#ifndef POSITION_HPP
#define POSITION_HPP

#include <array>
#include <cstdint>
#include <list>
#include <string>
#include <vector>

#include "castling.hpp"
#include "precomputed.hpp"
#include "square.hpp"
#include "types.hpp"

constexpr auto DEFAULT_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
constexpr u8 BITBOARDS_NUMBER = 12;

class Position {
   public:
    Position(const std::string &fen = DEFAULT_FEN) {
        setFromFen(fen);
        precomputeAttacks();
    }

    void setFromFen(const std::string &fen);
    [[nodiscard]] std::string toFen() const;

    void setPiece(Square s, Piece p);
    void unsetPiece(Square s);
    void resetBoard();
    [[nodiscard]] Piece pieceAt(Square s) const;
    [[nodiscard]] bool isEmpty(Square s) const;
    [[nodiscard]] bool isAlly(Square s) const;
    [[nodiscard]] Bitboard getAllyBb(Color us) const;
    [[nodiscard]] bool isOpponent(Square s) const;
    [[nodiscard]] bool isOpponent(Square s, PieceType pt) const;
    [[nodiscard]] bool isOpponent(Square s, Piece p) const;

    [[nodiscard]] bool isLegal();

    void generatePawnMoves(std::vector<Move> &moves, Square s) const;
    void generateKnightMoves(std::vector<Move> &moves, Square s) const;
    void generateBishopMoves(std::vector<Move> &moves, Square s) const;
    void generateRookMoves(std::vector<Move> &moves, Square s) const;
    void generateQueenMoves(std::vector<Move> &moves, Square s) const;
    void generateKingMoves(std::vector<Move> &moves, Square s) const;

    void generatePseudoLegalMoves(std::vector<Move> &moves);
    int perft(u8 depth, bool verbose = false);

    void doMove(Move move);
    void undoMove();
    [[nodiscard]] Color turn() const { return m_turn; }

   private:
    std::array<Bitboard, BITBOARDS_NUMBER> m_bitboards{0ULL};

    std::list<Delta> m_deltas;
    std::list<Move> m_moves;

    Color m_turn = Color::WHITE;
    Castling m_castling = Castling::ANY;
    u8 m_halfmoves = 0;
};

#endif