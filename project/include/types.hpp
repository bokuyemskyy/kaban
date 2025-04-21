#ifndef TYPES_HPP
#define TYPES_HPP

#include <cassert>
#include <cstdint>
#include <utility>

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using Move = u16;
using Delta = u32;
using Bitboard = u64;
using Direction = std::pair<u8, u8>;

#endif