#ifndef SQUARE_HPP
#define SQUARE_HPP

#include <array>
#include <cassert>
#include <cstdint>

#include "piece.hpp"

// clang-format off

using Square    = uint8_t;
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
    MASK = 0b111111,
    SIZE = 6,
    NB = 64
};
}  // namespace Squares

using File      = uint8_t;
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
}  // namespace Files

using Rank      = uint8_t;
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
}  // namespace Ranks

// clang-format on

inline Square createSquare(File file, Rank rank) { return rank << Files::SIZE | file; }
inline File   getFile(Square square) { return square & Files::MASK; }
inline Rank   getRank(Square square) { return square >> Files::SIZE; }

inline bool isPawnStartingRank(Rank rank, Color color) {
    return (color == Colors::WHITE && rank == Ranks::R2) || (color == Colors::BLACK && rank == Ranks::R7);
}
inline bool isPawnPromotionRank(Rank rank, Color color) {
    return (color == Colors::WHITE && rank == Ranks::R8) || (color == Colors::BLACK && rank == Ranks::R1);
}

constexpr bool isValid(Square square) { return square <= Squares::LAST; }

extern std::array<std::array<uint8_t, Squares::NB>, Squares::NB> squareDistance;

void initSquares();

#endif