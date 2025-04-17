#include "navigation.hpp"

#include <cstdint>

#include "types.hpp"

Square turnSquare(Square screenSquare, Color turn) {
    if (screenSquare == Square::NONE) return Square::NONE;
    Rank r = turn == Color::WHITE
                 ? getRank(screenSquare)
                 : static_cast<Rank>(BOARD_SIZE - 1 - static_cast<u8>(getRank(screenSquare)));
    File f = turn == Color::WHITE
                 ? getFile(screenSquare)
                 : static_cast<File>(BOARD_SIZE - 1 - static_cast<u8>(getFile(screenSquare)));
    return makeSquare(f, r);
}