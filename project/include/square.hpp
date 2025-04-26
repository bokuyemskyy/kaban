#ifndef SQUARE_HPP
#define SQUARE_HPP

#include <cassert>
#include <cstdint>
#include <string>

#include "piece.hpp"

// clang-format off

/* ############### Definitions ############### */

// ##### Bitboard #####

using Bitboard                   = uint64_t;
constexpr Bitboard BITBOARD_ZERO = 0ULL;

// ##### Move #####

using Move = uint16_t;

// ##### Delta #####

using Delta = uint32_t;

// ##### Square #####

using Square = uint8_t;
namespace Squares {
enum value : uint8_t {
    A1, B1, C1, D1, E1, F1, G1, H1,
    A2, B2, C2, D2, E2, F2, G2, H2,
    A3, B3, C3, D3, E3, F3, G3, H3,
    A4, B4, C4, D4, E4, F4, G4, H4,
    A5, B5, C5, D5, E5, F5, G5, H5,
    A6, B6, C6, D6, E6, F6, G6, H6,
    A7, B7, C7, D7, E7, F7, G7, H7,
    A8, B8, C8, D8, E8, F8, G8, H8,

    FIRST = A1,
    LAST = H8,

    NONE = 64,
    NB = 64
};
}

// ##### File #####

using File = uint8_t;
namespace Files {
enum value : uint8_t { 
    FA, FB, FC, FD, FE, FF, FG, FH, 
    
    FIRST = FA,
    LAST  = FH,
    
    NONE = 8,
    MASK = 0b111,
    SIZE = 3,
    NB   = 8 
};
}

// ##### Rank #####

using Rank = uint8_t;
namespace Ranks {
enum value : uint8_t { 
    R1, R2, R3, R4, R5, R6, R7, R8,
    
    FIRST = R1,
    LAST  = R8,
    
    NONE = 8,
    MASK = 0b111,
    SIZE = 3,
    NB   = 8
};
}

// ##### Direction #####

using Direction = int8_t;
namespace Directions {
enum value : int8_t { 
    EAST = 1, 
    NORTH = 8,
    WEST = -1,
    SOUTH = -8,

    NORTH_EAST = NORTH + EAST,
    SOUTH_EAST = SOUTH + EAST,
    NORTH_WEST = NORTH + WEST,
    SOUTH_WEST = SOUTH + WEST,
    
    NONE = 0,
    NB   = 4
};
}

// ##### Castling #####

using Castling = uint8_t;
namespace Castlings {
enum value : uint8_t {
    WKINGSIDE  = 0b0001,
    WQUEENSIDE = 0b0010,
    BKINGSIDE  = 0b0100,
    BQUEENSIDE = 0b1000,

    KINGSIDE  = WKINGSIDE | BKINGSIDE,    // 0b0101
    QUEENSIDE = WQUEENSIDE | BQUEENSIDE,  // 0b1010
    WSIDE     = WKINGSIDE | WQUEENSIDE,   // 0b0011
    BSIDE     = BKINGSIDE | BQUEENSIDE,   // 0b1100
    ANY       = WSIDE | BSIDE,            // 0b1111

    NONE = 0b0000,

    MASK = 0b1111,
    SIZE = 4,
    NB   = 4
};
}  // namespace Castlings

/* ############### Helpers ############### */

// ##### File #####

inline File getFile(Square square) { return square & Files::MASK; }

// ##### Rank #####

inline Rank getRank(Square square) { return square >> Files::SIZE; }

inline bool isPawnStartingRank(Rank rank, Color color) {
    return (color == Colors::WHITE && rank == Ranks::R2) || (color == Colors::BLACK && rank == Ranks::R7);
}
inline bool isPawnPromotionRank(Rank rank, Color color) {
    return (color == Colors::WHITE && rank == Ranks::R8) || (color == Colors::BLACK && rank == Ranks::R1);
}

// ##### Square #####

inline Square makeSquare(File file, Rank rank) {
    return rank << Files::SIZE | file;
}
inline bool isSquareInBounds(Square square) {
    return square <= Squares::LAST;
}
constexpr Bitboard squareBB(Square square) {
    assert(isSquareInBounds(square));
    return (1ULL << square);
}
constexpr std::string fileToChar(File file) {
    return {1, static_cast<char>('a' + file)};
}

constexpr std::string rankToChar(Rank rank) {
    return {1, static_cast<char>('1' + rank)};
}
inline std::string squareToString(Square s) {
    return fileToChar(getFile(s)) + rankToChar(getRank(s));
}

// ##### Move #####

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

inline Move createMove(Square from, Square to, uint8_t flags) {
    return (static_cast<uint8_t>(from) & 0x3F) | ((static_cast<uint8_t>(to) & 0x3F) << 6) | ((flags & 0xF) << 12);
}

inline Square  getFrom(Move move) { return static_cast<Square>(move & 0x3F); }
inline Square  getTo(Move move) { return static_cast<Square>((move >> 6) & 0x3F); }
inline uint8_t getFlags(Move move) { return (move >> 12) & 0xF; }

// ##### Delta #####

inline Delta createDelta(Piece captured, Castling castling, uint8_t enpassant, uint8_t halfmoves) {
    return (static_cast<uint8_t>(captured) & 0xF) | ((static_cast<uint8_t>(castling) & 0xF) << 4) |
           ((enpassant & 0x3F) << 8) | ((halfmoves & 0xFF) << 14);
}

inline Piece getCaptured(Delta delta) { return Piece(delta & 0xF); }

inline Castling getCastling(Delta delta) { return Castling((delta >> 4) & 0xF); }

inline uint8_t getEnpassant(Delta delta) { return (delta >> 8) & 0x3F; }

inline uint8_t getHalfmoves(Delta delta) { return (delta >> 14) & 0xFF; }

// clang-format on

#endif