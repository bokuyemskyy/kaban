#include "navigation.hpp"

#include <cstdint>

#include "types.hpp"

Square getTurnSquare(Square screenSquare, Turn turn) {
    if (screenSquare == Square::NONE) return Square::NONE;
    Rank r = turn == Turn::WHITE
                 ? getRank(screenSquare)
                 : static_cast<Rank>(BOARD_SIZE - 1 - static_cast<uint8_t>(getRank(screenSquare)));
    File f = turn == Turn::WHITE
                 ? getFile(screenSquare)
                 : static_cast<File>(BOARD_SIZE - 1 - static_cast<uint8_t>(getFile(screenSquare)));
    return makeSquare(f, r);
}