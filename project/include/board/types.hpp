#ifndef TYPES_HPP
#define TYPES_HPP

#include <cstdint>

using Square    = uint8_t;
using File      = uint8_t;
using Rank      = uint8_t;
using Direction = int8_t;
using Castling  = uint8_t;
using Flags     = uint8_t;
using Move      = uint16_t;
using Delta     = uint32_t;
using Bitboard  = uint64_t;
using Magic     = Bitboard;

#endif