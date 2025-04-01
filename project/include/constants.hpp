#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <cstdint>

// clang-format off

#define MAX_MOVES 256
#define BOARD_SIZE 8

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