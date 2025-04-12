#ifndef POSITION_HPP
#define POSITION_HPP

#include <array>
#include <cstdint>
#include <list>
#include <string>
#include <vector>

#include "bitboard.hpp"
#include "types.hpp"

constexpr auto DEFAULT_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";

class Position {
   public:
    Position(const std::string &fen = DEFAULT_FEN) : m_castling(Castling::ANY), m_halfmoves(0) {
        setFEN(fen);

        m_possibleMoves.reserve(MAX_MOVES);
    }

    void setFEN(const std::string &fen);
    std::string getFEN() const;

    void setPiece(Square s, Piece p);
    void unsetPiece(Square s);
    [[nodiscard]] Piece pieceAt(Square s) const;

    void generatePossibleMoves();

    void makeMove(Move move);
    void unmakeMove();
    [[nodiscard]] Turn getTurn() const { return m_turn; }

   private:
    std::array<Bitboard, BITBOARDS_NUMBER> m_bitboards;

    std::vector<Move> m_possibleMoves;

    std::list<Delta> m_deltas;
    std::list<Move> m_moves;

    Turn m_turn = Turn::WHITE;
    Castling m_castling;
    uint8_t m_halfmoves;
};

#endif