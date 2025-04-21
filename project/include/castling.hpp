#ifndef CASTLING_HPP
#define CASTLING_HPP

#include "types.hpp"

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

#endif