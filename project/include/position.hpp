#ifndef POSITION_HPP
#define POSITION_HPP

#include <array>
#include <cstdint>
#include <string>
#include <types.hpp>
#include <vector>

#include "constants.hpp"

inline bool isWhite(Piece piece) { return (piece <= 5); }

struct GameState {
    CastlingRights castlingRights;
    uint8_t movesRepeated;
    uint8_t movesRule50;

    GameState *next;
    GameState *previous;
};

class Bitboard {
   public:
    uint64_t _value;

    Bitboard() : _value(0ULL) {}
    Bitboard(uint64_t value) : _value(value) {}

    bool isSet(uint8_t square) const;
    void set(uint8_t square);
    void unset(uint8_t square);
};

class Position {
   public:
    Position() {
        m_possibleMoves.reserve(MAX_MOVES);
        m_gameState = new GameState{ANY_CASTLING, 0, 0, nullptr, nullptr};
    }
    void setFEN(std::string fen);

    void setPiece(uint8_t square, Piece piece);
    void unsetPiece(uint8_t square);
    Piece getPiece(uint8_t square) const;
    void generatePossibleMoves();
    void makeMove(const Move &move);
    void unmakeMove();
    void updateExternalData();
    const std::array<std::array<Piece, 8>, 8> &getMatrixBoard();
    const std::vector<Move> &getPossibleMoves();
    Turn getTurn() const { return m_turn; }

   private:
    bool m_isExternalDataDirty = true;
    std::array<std::array<Piece, 8>, 8> m_matrixBoard;
    std::vector<Move> m_possibleMoves;

    Bitboard m_piece_bitboards[12];
    Bitboard m_white_occupied;
    Bitboard m_black_occupied;
    Bitboard m_occupied;

    Turn m_turn = WHITE;

    GameState *m_gameState;
};

#endif