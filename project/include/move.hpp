#ifndef MOVE_HPP
#define MOVE_HPP

#include "castling.hpp"
#include "piece.hpp"
#include "square.hpp"
#include "types.hpp"

// ########## MOVES ##########

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

inline Move createMove(Square from, Square to, u8 flags) {
    return (static_cast<u8>(from) & 0x3F) | ((static_cast<u8>(to) & 0x3F) << 6) |
           ((flags & 0xF) << 12);
}

inline Square getFrom(Move move) { return static_cast<Square>(move & 0x3F); }
inline Square getTo(Move move) { return static_cast<Square>((move >> 6) & 0x3F); }
inline u8 getFlags(Move move) { return (move >> 12) & 0xF; }

// ########## DELTAS ##########

inline Delta createDelta(Piece captured, Castling castling, u8 enpassant, u8 halfmoves) {
    return (static_cast<u8>(captured) & 0xF) | ((static_cast<u8>(castling) & 0xF) << 4) |
           ((enpassant & 0x3F) << 8) | ((halfmoves & 0xFF) << 14);
}

inline Piece getCaptured(Delta delta) { return Piece(delta & 0xF); }

inline Castling getCastling(Delta delta) { return Castling((delta >> 4) & 0xF); }

inline u8 getEnpassant(Delta delta) { return (delta >> 8) & 0x3F; }

inline u8 getHalfmoves(Delta delta) { return (delta >> 14) & 0xFF; }

#endif