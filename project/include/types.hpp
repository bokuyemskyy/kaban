#ifndef TYPES_HPP
#define TYPES_HPP

#include <cassert>
#include <cstdint>

// clang-format off

// ########## RENAME ##########

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using Move = u16;
using Delta = u32;
using Bitboard = u64;

// ########## CONSTANTS ##########

constexpr u8 MAX_MOVES = 256;
constexpr u8 BOARD_SIZE = 8;
constexpr u8 BOARD_SQUARES = BOARD_SIZE * BOARD_SIZE;

// ########## PIECES ##########

enum class PieceType : u8 {
  PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING, 
};
constexpr u8 PIECETYPE_NB = 6;

enum class Color : bool {
  WHITE, BLACK
};

enum class Piece : u8 {
  WPAWN, WKNIGHT, WBISHOP, WROOK, WQUEEN, WKING,

  BPAWN, BKNIGHT, BBISHOP, BROOK, BQUEEN, BKING,
  
  NONE
};
constexpr u8 PIECE_NB = 12;

inline Piece createPiece(Color c, PieceType pt) {
  return static_cast<Piece>(static_cast<u8>(pt)+(static_cast<u8>(c)*PIECETYPE_NB));
}

inline Color getColor(Piece p) {
  assert(p != Piece::NONE);
  return static_cast<Color>((static_cast<u8>(p) % PIECETYPE_NB != static_cast<u8>(p)));
}

inline PieceType getPieceType(Piece p) {
  assert(p != Piece::NONE);
  return static_cast<PieceType>(static_cast<u8>(p) % PIECETYPE_NB); 
}

constexpr Piece charToPiece(char c) {
    switch (c) {
        case 'P': return Piece::WPAWN;
        case 'N': return Piece::WKNIGHT;
        case 'B': return Piece::WBISHOP;
        case 'R': return Piece::WROOK;
        case 'Q': return Piece::WQUEEN;
        case 'K': return Piece::WKING;
        case 'p': return Piece::BPAWN;
        case 'n': return Piece::BKNIGHT;
        case 'b': return Piece::BBISHOP;
        case 'r': return Piece::BROOK;
        case 'q': return Piece::BQUEEN;
        case 'k': return Piece::BKING;
        default:  return Piece::NONE;
    }
}

constexpr char pieceToChar(Piece p) {
  switch (p) {
    case Piece::WPAWN:   return 'P';
    case Piece::WKNIGHT: return 'N';
    case Piece::WBISHOP: return 'B';
    case Piece::WROOK:   return 'R';
    case Piece::WQUEEN:  return 'Q';
    case Piece::WKING:   return 'K';
    case Piece::BPAWN:   return 'p';
    case Piece::BKNIGHT: return 'n';
    case Piece::BBISHOP: return 'b';
    case Piece::BROOK:   return 'r';
    case Piece::BQUEEN:  return 'q';
    case Piece::BKING:   return 'k';
    default:             return '?';
  }
}
    
// ########## SQUARES ##########

enum class Square : u8 {
  A1, B1, C1, D1, E1, F1, G1, H1,    
  A2, B2, C2, D2, E2, F2, G2, H2,
  A3, B3, C3, D3, E3, F3, G3, H3,
  A4, B4, C4, D4, E4, F4, G4, H4,
  A5, B5, C5, D5, E5, F5, G5, H5,
  A6, B6, C6, D6, E6, F6, G6, H6,
  A7, B7, C7, D7, E7, F7, G7, H7,
  A8, B8, C8, D8, E8, F8, G8, H8, 
  NONE
};

enum class File : u8 { FA, FB, FC, FD, FE, FF, FG, FH };
enum class Rank : u8 { R1, R2, R3, R4, R5, R6, R7, R8 };

inline File getFile(Square s) {
    return static_cast<File>(static_cast<u8>(s) & (0b111));
}

inline Rank getRank(Square s) {
    return static_cast<Rank>(static_cast<u8>(s) >> 3);
}

inline Square makeSquare(File f, Rank r) {
    return static_cast<Square>(static_cast<u8>(r) << 3 | static_cast<u8>(f));
}

// ########## CASTLING ##########

enum class Castling : u8 {
  WKINGSIDE = 1 << 0,
  WQUEENSIDE = 1 << 1,
  BKINGSIDE = 1 << 2,
  BQUEENSIDE = 1 << 3,
  
  KINGSIDE = WKINGSIDE | BKINGSIDE,
  QUEENSIZE = WQUEENSIDE | BQUEENSIDE,
  WSIDE = WKINGSIDE | WQUEENSIDE,
  BSIDE = BKINGSIDE | BQUEENSIDE,
  ANY = WSIDE | BSIDE,

  NONE
};

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
    return (static_cast<u8>(from) & 0x3F) | ((static_cast<u8>(to) & 0x3F) << 6) | ((flags & 0xF) << 12);
}

inline Square getFrom(Move move) { return static_cast<Square>(move & 0x3F); }
inline Square getTo(Move move) { return static_cast<Square>((move >> 6) & 0x3F); }
inline u8 getFlags(Move move) { return (move >> 12) & 0xF; }

// ########## DELTAS ##########

inline Delta createDelta(Piece captured, Castling castling, u8 enpassant, u8 halfmoves) {
    return (static_cast<u8>(captured) & 0xF) | 
           ((static_cast<u8>(castling) & 0xF) << 4) | 
           ((enpassant & 0x3F) << 8) | 
           ((halfmoves & 0xFF) << 14);
}

inline Piece getCaptured(Delta delta) {
    return Piece(delta & 0xF);
}

inline Castling getCastling(Delta delta) {
    return Castling((delta >> 4) & 0xF);
}

inline u8 getEnpassant(Delta delta) {
    return (delta >> 8) & 0x3F;
}

inline u8 getHalfmoves(Delta delta) {
    return (delta >> 14) & 0xFF;
  }

// clang-format on

#endif