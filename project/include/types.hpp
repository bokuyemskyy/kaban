#ifndef TYPES_HPP
#define TYPES_HPP

#include <cstdint>
#include <unordered_map>

// clang-format off

  
enum Piece : uint8_t {
  WPAWN, WKNIGHT, WBISHOP, WROOK, WQUEEN, WKING, 
  BPAWN, BKNIGHT, BBISHOP, BROOK, BQUEEN, BKING,
  EMPTY
};
  

enum CastlingRights : uint8_t {
  NO_CASTLING,
  WHITE_OO,
  WHITE_OOO = WHITE_OO << 1,
  BLACK_OO = WHITE_OO << 2,
  BLACK_OOO = WHITE_OO << 3,

  KING_SIDE = WHITE_OO | BLACK_OO,
  QUEEN_SIDE = WHITE_OOO | BLACK_OOO,
  WHITE_CASTLING = WHITE_OO | WHITE_OOO,
  BLACK_CASTLING = BLACK_OO | BLACK_OOO,
  ANY_CASTLING = WHITE_CASTLING | BLACK_CASTLING
};
  
enum Turn : bool { WHITE, BLACK };
  
static const std::unordered_map<char, Piece> FENtoPiece = {
  {'P', WPAWN},   {'N', WKNIGHT}, {'B', WBISHOP}, {'R', WROOK},
  {'Q', WQUEEN},  {'K', WKING},   {'p', BPAWN},   {'n', BKNIGHT},
  {'b', BBISHOP}, {'r', BROOK},   {'q', BQUEEN},  {'k', BKING}};
static const std::unordered_map<Piece, char> PieceToFEN = {
  {WPAWN, 'P'},   {WKNIGHT, 'N'}, {WBISHOP, 'B'}, {WROOK, 'R'},
  {WQUEEN, 'Q'},  {WKING, 'K'},   {BPAWN, 'p'},   {BKNIGHT, 'n'},
  {BBISHOP, 'b'}, {BROOK, 'r'},   {BQUEEN, 'q'},  {BKING, 'k'}};

// clang-format on

struct Move {
    uint8_t from : 6;
    uint8_t to : 6;
    uint8_t replaced : 4;
    uint8_t promotion : 3;
    uint8_t castling : 3;
    uint8_t enPassant : 1;

    Move() : from(0), to(0), replaced(0), promotion(0), castling(0), enPassant(0) {}
    Move(uint8_t from, uint8_t to, uint8_t replaced, uint8_t promotion = 0, uint8_t castling = 0,
         uint8_t enPassant = 0)
        : from(from),
          to(to),
          replaced(replaced),
          promotion(promotion),
          castling(castling),
          enPassant(enPassant) {}
};

#endif