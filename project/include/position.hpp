#ifndef POSITION_HPP
#define POSITION_HPP

#include <array>
#include <cstdint>
#include <list>
#include <string>
#include <vector>

#include "types.hpp"

constexpr auto DEFAULT_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
constexpr uint8_t BITBOARDS_NUMBER = 12;

class Position {
   public:
    Position(const std::string &fen = DEFAULT_FEN) : m_castling(Castling::ANY), m_halfmoves(0) {
        setFen(fen);

        m_possibleMoves.reserve(MAX_MOVES);
    }

    void setFen(const std::string &fen);
    std::string getFen() const;

    void setPiece(Square s, Piece p);
    void unsetPiece(Square s);
    [[nodiscard]] Piece pieceAt(Square s) const;

    void generateMoves();
    void generatePseudoMoves();
    int perft(uint8_t depth);

    void doMove(Move move);
    void undoMove();
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