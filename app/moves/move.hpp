#pragma once

#include "navigation.hpp"
#include "piece.hpp"
struct Flag : Metadata<Flag> {
   public:
    enum : uint8_t {
        USUAL,
        PROMOTION,
        DOUBLE_PUSH,
        CASTLING,
    };

    constexpr Flag(uint8_t value) : m_value(value) {}

    [[nodiscard]] constexpr operator uint8_t() const { return m_value; }

    enum : uint8_t {
        NB   = 4,
        SIZE = 2,
        MASK = (1 << SIZE) - 1
    };

   private:
    uint8_t m_value;
};

struct Promotion : Metadata<Promotion> {
   public:
    enum : uint8_t {
        TO_KNIGHT,
        TO_BISHOP,
        TO_ROOK,
        TO_QUEEN,
    };

    constexpr Promotion(uint8_t value) : m_value(value) {}

    [[nodiscard]] constexpr operator uint8_t() const { return m_value; }

    enum : uint8_t {
        NB   = 4,
        SIZE = 2,
        MASK = (1 << SIZE) - 1
    };

   private:
    uint8_t m_value;
};

// Move layout (16 bits):
// [15..14] Promotion (2 bits)
// [13..12] Flag      (2 bits)
// [11..6 ] To square (6 bits)
// [5 ..0 ] From square (6 bits)

// I am going to test out: is there really some actual difference in such an approach (bit-packing) and bit-field
// structures
struct Move {
   public:
    constexpr Move() : m_value(0) {}
    constexpr Move(uint16_t value) : m_value(value) {}

    constexpr Move(Square from, Square to, Flag flag = Flag::USUAL, Promotion promotion = Promotion::TO_QUEEN)
        : m_value((promotion << PROMOTION_SHIFT) | (flag << FLAG_SHIFT) | (to.value() << TO_SHIFT) | from.value()) {}

    [[nodiscard]] constexpr Square from() const { return Square(m_value & Square::mask()); }

    [[nodiscard]] constexpr Square to() const { return Square((m_value >> TO_SHIFT) & Square::mask()); }

    [[nodiscard]] constexpr Flag flag() const { return Flag((m_value >> FLAG_SHIFT) & Flag::mask()); }

    [[nodiscard]] constexpr Promotion promotion() const {
        return Promotion((m_value >> PROMOTION_SHIFT) & Promotion::mask());
    }

    constexpr operator uint16_t() const { return m_value; }

    [[nodiscard]] constexpr bool hasValue() const { return m_value != 0; }

   private:
    uint16_t m_value;

    static constexpr int FROM_SHIFT      = 0;
    static constexpr int TO_SHIFT        = Square::size();
    static constexpr int FLAG_SHIFT      = 2 * Square::size();
    static constexpr int PROMOTION_SHIFT = (2 * Square::size()) + Flag::size();
};
