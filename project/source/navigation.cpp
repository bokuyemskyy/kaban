#include <cstdint>
#include <navigation.hpp>
#include <types.hpp>

#include "constants.hpp"

uint8_t getTurnSquare(uint8_t screenSquare, Turn turn) {
    if (screenSquare == NO_SQ) return NO_SQ;
    uint8_t newRow = turn == WHITE ? (screenSquare >> 3) : 7 - (screenSquare >> 3);
    uint8_t newCol = turn == WHITE ? (screenSquare % 8) : 7 - (screenSquare % 8);
    return Square(newRow * 8 + newCol);
}