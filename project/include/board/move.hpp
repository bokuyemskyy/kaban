#pragma once

#include <cstdint>

#include "castling.hpp"
#include "navigation.hpp"

#pragma once
#include <cassert>
#include <cstdint>

struct Flag {
   public:
    enum Value : uint8_t {
        USUAL,
        PROMOTION,
        DOUBLE_PUSH,
        CASTLING,

        NB   = 4,
        SIZE = 2,
        MASK = (1 << SIZE) - 1
    };

    constexpr Flag(Value value) : m_value(value) {}
    constexpr Flag(uint8_t value) : m_value(static_cast<Value>(value)) {}

    [[nodiscard]] constexpr operator uint8_t() const { return m_value; }

    [[nodiscard]] static constexpr uint8_t size() { return SIZE; }
    [[nodiscard]] static constexpr uint8_t mask() { return MASK; }
    [[nodiscard]] static constexpr uint8_t number() { return NB; }

   private:
    Value m_value;
};

struct Promotion {
   public:
    enum Value : uint8_t {
        TO_KNIGHT,
        TO_BISHOP,
        TO_ROOK,
        TO_QUEEN,

        NB   = 4,
        SIZE = 2,
        MASK = (1 << SIZE) - 1
    };

    constexpr Promotion(Value value) : m_value(value) {}
    constexpr Promotion(uint8_t value) : m_value(static_cast<Value>(value)) {}

    [[nodiscard]] constexpr operator uint8_t() const { return m_value; }

    [[nodiscard]] static constexpr uint8_t size() { return SIZE; }
    [[nodiscard]] static constexpr uint8_t mask() { return MASK; }
    [[nodiscard]] static constexpr uint8_t number() { return NB; }

   private:
    Value m_value;
};

// Move layout (16 bits):
// [15..14] Promotion (2 bits)
// [13..12] Flag      (2 bits)
// [11..6 ] To square (6 bits)
// [5 ..0 ] From square (6 bits)

struct Move {
   public:
    constexpr Move() : m_value(0) {}
    constexpr Move(uint16_t value) : m_value(value) {}

    constexpr Move(Square from, Square to, Flag flag = Flag::USUAL, Promotion promotion = Promotion::TO_QUEEN)
        : m_value((promotion << PROMO_SHIFT) | (flag << FLAG_SHIFT) | (to << TO_SHIFT) | from) {}

    [[nodiscard]] constexpr Square from() const { return Square(m_value & Square::mask()); }

    [[nodiscard]] constexpr Square to() const { return Square((m_value >> TO_SHIFT) & Square::mask()); }

    [[nodiscard]] constexpr Flag flag() const { return Flag((m_value >> FLAG_SHIFT) & Flag::mask()); }

    [[nodiscard]] constexpr Promotion promotion() const {
        return Promotion((m_value >> PROMO_SHIFT) & Promotion::mask());
    }

    constexpr operator uint16_t() const { return m_value; }

    [[nodiscard]] constexpr bool isNull() const { return m_value == 0; }

   private:
    uint16_t m_value;

    static constexpr int FROM_SHIFT  = 0;
    static constexpr int TO_SHIFT    = Square::size();
    static constexpr int FLAG_SHIFT  = 2 * Square::size();
    static constexpr int PROMO_SHIFT = FLAG_SHIFT + Flag::size();
};

// Delta layout (32 bits):
// [3 ..0 ] Captured Piece (4 bits)
// [7 ..4 ] Castling Rights (4 bits)
// [13..8 ] En Passant Square (6 bits)
// [21..14] Halfmove Clock (8 bits)
// [23..22] Extra Flags (2 bits)

struct Delta {
   public:
    constexpr Delta(Piece captured, Castling castling, uint8_t enpassant, uint8_t halfmoves, uint8_t extraFlags = 0)
        : m_value((captured & 0xF) | ((castling & 0xF) << 4) | ((enpassant & 0x3F) << 8) | ((halfmoves & 0xFF) << 14) |
                  ((extraFlags & 0x3) << 22)) {}

    constexpr Delta(uint32_t raw) : m_value(raw) {}

    constexpr operator uint32_t() const { return m_value; }

    [[nodiscard]] constexpr Piece    captured() const { return m_value & 0xF; }
    [[nodiscard]] constexpr Castling castling() const { return (m_value >> 4) & 0xF; }
    [[nodiscard]] constexpr uint8_t  enpassant() const { return (m_value >> 8) & 0x3F; }
    [[nodiscard]] constexpr uint8_t  halfmoves() const { return (m_value >> 14) & 0xFF; }
    [[nodiscard]] constexpr uint8_t  extraFlags() const { return (m_value >> 22) & 0x3; }

   private:
    uint32_t m_value;
};
