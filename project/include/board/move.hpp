// HAS TESTS

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

constexpr Move createMove(Square from, Square to, Flags flags) {
    return from | (to << Squares::SIZE) | (flags << (Squares::SIZE << 2));
}

constexpr Square getFrom(Move move) { return move & Squares::MASK; }
constexpr Square getTo(Move move) { return (move >> Squares::SIZE) & Squares::MASK; }
constexpr Flags  getFlags(Move move) { return move >> (Squares::SIZE << 2); }

constexpr Delta createDelta(Piece captured, Castling castling, uint8_t enpassant, uint8_t halfmoves,
                            uint8_t extraFlags = 0) {
    return static_cast<Delta>((captured & 0xF) | ((castling & 0xF) << 4) | ((enpassant & 0x3F) << 8) |
                              ((halfmoves & 0xFF) << 14) | ((extraFlags & 0x3) << 22));
}

constexpr Piece    getCaptured(Delta delta) { return Piece(delta & 0xF); }
constexpr Castling getCastling(Delta delta) { return Castling((delta >> 4) & 0xF); }
constexpr uint8_t  getEnpassant(Delta delta) { return (delta >> 8) & 0x3F; }
constexpr uint8_t  getHalfmoves(Delta delta) { return (delta >> 14) & 0xFF; }
constexpr uint8_t  getExtraFlags(Delta delta) { return (delta >> 22) & 0x3; }

#endif