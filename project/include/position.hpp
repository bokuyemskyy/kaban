#ifndef POSITION_HPP
#define POSITION_HPP

#include <array>
#include <cstdint>
#include <list>
#include <string>
#include <vector>

#include "types.hpp"

constexpr auto DEFAULT_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
constexpr u8 BITBOARDS_NUMBER = 12;

class Position {
   public:
    Position(const std::string &fen = DEFAULT_FEN) { setFromFen(fen); }

    void setFromFen(const std::string &fen);
    [[nodiscard]] std::string toFen() const;

    void setPiece(Square s, Piece p);
    void unsetPiece(Square s);
    [[nodiscard]] Piece pieceAt(Square s) const;

    [[nodiscard]] bool isLegal();
    void generatePseudoLegalMoves(std::vector<Move> &moveList);
    u64 perft(u8 depth);

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