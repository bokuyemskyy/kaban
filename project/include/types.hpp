#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <cstdint>

// clang-format off

constexpr uint8_t MAX_MOVES = 256;
constexpr uint8_t BOARD_SIZE = 8;

enum class Piece : uint8_t {
  WPAWN,
  WKNIGHT,
  WBISHOP,
  WROOK,
  WQUEEN,
  WKING,

  BPAWN,
  BKNIGHT,
  BBISHOP,
  BROOK,
  BQUEEN,
  BKING,

  NONE,

  FIRST = WPAWN,
  LAST = BKING
};

enum class Color : uint8_t {
  WHITE, BLACK, NONE
};

constexpr Color getColor(Piece p) {
  if (p == Piece::NONE) return Color::NONE;
  auto val = static_cast<uint8_t>(p);
  return static_cast<Color>((val & static_cast<uint8_t>(Piece::BPAWN)) >> 3);
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
    
enum class Square : uint8_t {
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
 
enum class File : uint8_t { FA, FB, FC, FD, FE, FF, FG, FH };
enum class Rank : uint8_t { R1, R2, R3, R4, R5, R6, R7, R8 };

enum class Castling : uint8_t {
  WHITE_KING = 1 << 0,
  WHITE_QUEEN = 1 << 1,
  BLACK_KING = 1 << 2,
  BLACK_QUEEN = 1 << 3,
  NONE,
  
  KING = WHITE_KING | BLACK_KING,
  QUEEN = WHITE_QUEEN | BLACK_QUEEN,
  WHITE = WHITE_KING | WHITE_QUEEN,
  BLACK = BLACK_KING | BLACK_QUEEN,
  ANY = WHITE | BLACK
};

enum class Turn : bool { WHITE, BLACK };

using Move = uint16_t;

inline Move createMove(Square from, Square to, uint8_t flags) {
    return (static_cast<uint8_t>(from) & 0x3F) | ((static_cast<uint8_t>(to) & 0x3F) << 6) | ((flags & 0xF) << 12);
}

inline Square getFrom(Move move) { return static_cast<Square>(move & 0x3F); }
inline Square getTo(Move move) { return static_cast<Square>((move >> 6) & 0x3F); }
inline uint8_t getFlags(Move move) { return (move >> 12) & 0xF; }

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

using Delta = uint32_t;
inline Delta createDelta(Piece captured, Castling castling, uint8_t enpassant, uint8_t halfmoves) {
    return (static_cast<uint8_t>(captured) & 0xF) | 
           ((static_cast<uint8_t>(castling) & 0xF) << 4) | 
           ((enpassant & 0x3F) << 8) | 
           ((halfmoves & 0xFF) << 14);
}

inline Piece getCaptured(Delta delta) {
    return Piece(delta & 0xF);
}

inline Castling getCastling(Delta delta) {
    return Castling((delta >> 4) & 0xF);
}

inline uint8_t getEnpassant(Delta delta) {
    return (delta >> 8) & 0x3F;
}

inline uint8_t getHalfmoves(Delta delta) {
    return (delta >> 14) & 0xFF;
  }


constexpr File getFile(Square s) {
    return static_cast<File>(static_cast<uint8_t>(s) & (0b111));
}

constexpr Rank getRank(Square s) {
    return static_cast<Rank>(static_cast<uint8_t>(s) >> 3);
}

constexpr Square makeSquare(File f, Rank r) {
    return Square(static_cast<uint8_t>(r) << 3 | static_cast<uint8_t>(f));
}

// clang-format on

#endif