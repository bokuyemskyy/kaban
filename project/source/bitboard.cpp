#include "bitboard.hpp"

#include <cstdint>

#include "types.hpp"

bool Bitboard::isSet(Square s) const { return (m_value & (1ULL << static_cast<uint8_t>(s))) != 0; };

void Bitboard::set(Square s) { m_value |= (1ULL << static_cast<uint8_t>(s)); }

void Bitboard::unset(Square s) { m_value &= ~(1ULL << static_cast<uint8_t>(s)); }