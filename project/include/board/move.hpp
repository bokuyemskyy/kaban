#ifndef MOVE_HPP
#define MOVE_HPP

#include <cstdint>

#include "castling.hpp"
#include "square.hpp"

using Move  = uint16_t;
using Delta = uint32_t;
using Flags = uint8_t;

/*
0	Quiet move	No capture, no special move
1	Double pawn push	Pawn moves two squares forward
2	King castle	Kingside castling
3	Queen castle	Queenside castling
4	Capture (non-special)	Regular capture, piece on to-square
5	En passant capture	Capture via en passant, no piece on to-square initially
8	Knight promotion	Pawn promotes to knight
9	Bishop promotion	Pawn promotes to bishop
10	Rook promotion	Pawn promotes to rook
11	Queen promotion	Pawn promotes to queen
12	Knight promotion capture	Promotion with capture, to knight
13	Bishop promotion capture	Promotion with capture, to bishop
14	Rook promotion capture	Promotion with capture, to rook
15	Queen promotion capture	Promotion with capture, to queen
*/

inline Move createMove(Square from, Square to, Flags flags) {
    return from | (to << Squares::SIZE) | (flags << (Squares::SIZE << 2));
}

inline Square getFrom(Move move) { return move & Squares::MASK; }
inline Square getTo(Move move) { return (move >> Squares::SIZE) & Squares::MASK; }
inline Flags  getFlags(Move move) { return move >> (Squares::SIZE << 2); }

// ##### Delta #####

inline Delta createDelta(Piece captured, Castling castling, uint8_t enpassant, uint8_t halfmoves) {
    return (static_cast<uint8_t>(captured) & 0xF) | ((static_cast<uint8_t>(castling) & 0xF) << 4) |
           ((enpassant & 0x3F) << 8) | ((halfmoves & 0xFF) << 14);
}

inline Piece getCaptured(Delta delta) { return Piece(delta & 0xF); }

inline Castling getCastling(Delta delta) { return Castling((delta >> 4) & 0xF); }

inline uint8_t getEnpassant(Delta delta) { return (delta >> 8) & 0x3F; }

inline uint8_t getHalfmoves(Delta delta) { return (delta >> 14) & 0xFF; }

#endif