#pragma once

#include <cstdint>

#include "bitboard.hpp"
#include "square.hpp"
#ifdef _MSC_VER
#include <intrin0.inl.h>
#endif

inline Square lsb(const Bitboard& x) {
#ifdef _MSC_VER
    unsigned long index;
    _BitScanForward64(&index, x.value());
    return static_cast<Square>(static_cast<uint8_t>(index));
#else
    return static_cast<Square>(__builtin_ctzll(x.value()));
#endif
}

inline Square poplsb(Bitboard& x) {
    Square i = lsb(x);
    x        = Bitboard(x.value() & (x.value() - 1));
    return i;
}

inline uint8_t popcount(const Bitboard& x) {
#ifdef _MSC_VER
    return static_cast<uint8_t>(__popcnt64(x.value()));
#else
    return static_cast<uint8_t>(__builtin_popcountll(x.value()));
#endif
}
