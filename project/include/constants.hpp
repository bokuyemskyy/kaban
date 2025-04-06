#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <cstdint>
#include <unordered_map>

// clang-format off

constexpr uint8_t MAX_MOVES = 256;
constexpr uint8_t BOARD_SIZE = 8;

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

enum SquareEnum : uint8_t {
SQ_A1, SQ_B1, SQ_C1, SQ_D1, SQ_E1, SQ_F1, SQ_G1, SQ_H1,    
SQ_A2, SQ_B2, SQ_C2, SQ_D2, SQ_E2, SQ_F2, SQ_G2, SQ_H2,
SQ_A3, SQ_B3, SQ_C3, SQ_D3, SQ_E3, SQ_F3, SQ_G3, SQ_H3,
SQ_A4, SQ_B4, SQ_C4, SQ_D4, SQ_E4, SQ_F4, SQ_G4, SQ_H4,
SQ_A5, SQ_B5, SQ_C5, SQ_D5, SQ_E5, SQ_F5, SQ_G5, SQ_H5,
SQ_A6, SQ_B6, SQ_C6, SQ_D6, SQ_E6, SQ_F6, SQ_G6, SQ_H6,
SQ_A7, SQ_B7, SQ_C7, SQ_D7, SQ_E7, SQ_F7, SQ_G7, SQ_H7,
SQ_A8, SQ_B8, SQ_C8, SQ_D8, SQ_E8, SQ_F8, SQ_G8, SQ_H8, 
NO_SQ
};

using Square = SquareEnum;

enum File : uint8_t { FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H };
enum Rank : uint8_t { RANK_A, RANK_B, RANK_C, RANK_D, RANK_E, RANK_F, RANK_G, RANK_H };

constexpr Square makeSquare(File f, Rank r) {
    return Square(r << 3 | f);
}

// clang-format on

#endif