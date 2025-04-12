#ifndef BITBOARD_HPP
#define BITBOARD_HPP

#include <cstdint>

#include "types.hpp"

constexpr uint8_t BITBOARDS_NUMBER = 12;
class Bitboard {
    uint64_t m_value;

   public:
    Bitboard() : m_value(0ULL) {}
    Bitboard(uint64_t value) : m_value(value) {}

    [[nodiscard]] bool isSet(Square s) const;
    void set(Square s);
    void unset(Square s);
};

#endif