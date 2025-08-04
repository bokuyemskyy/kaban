#pragma once

#include "castling.hpp"
#include "navigation.hpp"

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

    [[nodiscard]] constexpr Square from() const { return m_value & Square::mask(); }

    [[nodiscard]] constexpr Square to() const { return (m_value >> TO_SHIFT) & Square::mask(); }

    [[nodiscard]] constexpr Flag flag() const { return (m_value >> FLAG_SHIFT) & Flag::mask(); }

    [[nodiscard]] constexpr Promotion promotion() const { return (m_value >> PROMO_SHIFT) & Promotion::mask(); }

    constexpr operator uint16_t() const { return m_value; }

    [[nodiscard]] constexpr bool empty() const { return m_value == 0; }

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
    constexpr Delta() : m_value(0) {}

    constexpr Delta(Piece captured, Castling castling, uint8_t enpassant, uint8_t halfmoves, uint8_t extraFlags = 0)
        : m_value((static_cast<uint32_t>(captured) << CAPTURE_SHIFT) |
                  (static_cast<uint32_t>(castling) << CASTLING_SHIFT) |
                  (static_cast<uint32_t>(enpassant) << ENPASSANT_SHIFT) |
                  (static_cast<uint32_t>(halfmoves) << HALFMOVES_SHIFT) |
                  (static_cast<uint32_t>(extraFlags) << EXTRA_SHIFT)) {}

    constexpr Delta(uint32_t raw) : m_value(raw) {}

    constexpr operator uint32_t() const { return m_value; }

    [[nodiscard]] constexpr Piece    captured() const { return (m_value >> CAPTURE_SHIFT) & CAPTURE_MASK; }
    [[nodiscard]] constexpr Castling castling() const {
        return static_cast<Castling>((m_value >> CASTLING_SHIFT) & CASTLING_MASK);
    }
    [[nodiscard]] constexpr uint8_t enpassant() const { return (m_value >> ENPASSANT_SHIFT) & ENPASSANT_MASK; }
    [[nodiscard]] constexpr uint8_t halfmoves() const { return (m_value >> HALFMOVES_SHIFT) & HALFMOVES_MASK; }
    [[nodiscard]] constexpr uint8_t extraFlags() const { return (m_value >> EXTRA_SHIFT) & EXTRA_MASK; }

   private:
    uint32_t m_value;

    static constexpr int CAPTURE_SHIFT   = 0;
    static constexpr int CASTLING_SHIFT  = 4;
    static constexpr int ENPASSANT_SHIFT = 8;
    static constexpr int HALFMOVES_SHIFT = 14;
    static constexpr int EXTRA_SHIFT     = 22;

    static constexpr uint32_t CAPTURE_MASK   = 0xF;
    static constexpr uint32_t CASTLING_MASK  = 0xF;
    static constexpr uint32_t ENPASSANT_MASK = 0x3F;
    static constexpr uint32_t HALFMOVES_MASK = 0xFF;
    static constexpr uint32_t EXTRA_MASK     = 0x3;
};