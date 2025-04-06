#ifndef NAVIGATION_HPP
#define NAVIGATION_HPP

#include <cstdint>

#include "constants.hpp"

uint8_t getTurnSquare(uint8_t screenSquare, Turn turn);

/*#include <cstdint>

#define Square uint8_t

const uint8_t SQ_NONE = uint8_t(1 << 7);

inline uint8_t createPos(uint8_t row, uint8_t col) {
    return (row << 4) | (col & 0x0F);
}

inline uint8_t getRow(uint8_t pos) {
    return (pos >> 4);
}

inline uint8_t getCol(uint8_t pos) {
    return (pos & 0x0F);
}

inline bool isNone(uint8_t pos) {
    return (pos >> 7);
}*/

#endif