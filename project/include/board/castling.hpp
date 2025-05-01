#ifndef CASTLING_HPP
#define CASTLING_HPP

#include <cstdint>

using Castling = uint8_t;
namespace Castlings {
enum value : std::uint8_t {
    WKINGSIDE  = 0b0001,
    WQUEENSIDE = 0b0010,
    BKINGSIDE  = 0b0100,
    BQUEENSIDE = 0b1000,

    KINGSIDE  = WKINGSIDE | BKINGSIDE,    // 0b0101
    QUEENSIDE = WQUEENSIDE | BQUEENSIDE,  // 0b1010
    WSIDE     = WKINGSIDE | WQUEENSIDE,   // 0b0011
    BSIDE     = BKINGSIDE | BQUEENSIDE,   // 0b1100
    ANY       = WSIDE | BSIDE,            // 0b1111

    NONE = 0b0000,

    MASK = 0b1111,
    SIZE = 4,
    NB   = 4
};
}  // namespace Castlings

#endif