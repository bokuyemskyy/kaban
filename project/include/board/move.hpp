// HAS TESTS

#ifndef MOVE_HPP
#define MOVE_HPP

#include <cstdint>

#include "navigation.hpp"

struct Flag {
   public:
    enum : uint8_t {
        USUAL,      // not a promotion, not a double pawn push, not a castling
        PROMOTION,  // pawn reached the last rank
        DOUBLE,     // double pawn push
        CASTLING,   // castling

        MASK = 0b11,
        SIZE = 2,
        NB   = 4
    };

    constexpr Flag(uint8_t value) : m_value(value) {}

    [[nodiscard]] static constexpr uint8_t number() { return NB; }
    [[nodiscard]] static constexpr uint8_t mask() { return MASK; }
    [[nodiscard]] static constexpr uint8_t size() { return SIZE; }

    constexpr operator uint8_t() const { return m_value; }

   private:
    uint8_t m_value;
};
struct Promotion {
   public:
    enum : uint8_t {
        TO_KNIGHT,
        TO_BISHOP,
        TO_ROOK,
        TO_QUEEN,

        MASK = 0b11,
        SIZE = 2,
        NB   = 4
    };

    constexpr Promotion(uint8_t value) : m_value(value) {}

    [[nodiscard]] static constexpr uint8_t number() { return NB; }
    [[nodiscard]] static constexpr uint8_t mask() { return MASK; }
    [[nodiscard]] static constexpr uint8_t size() { return SIZE; }

    constexpr operator uint8_t() const { return m_value; }

   private:
    uint8_t m_value;
};
struct Move {
   public:
    constexpr Move(uint16_t value) : m_value(value) {};
    constexpr Move(Square from, Square to, Flag flag, Promotion promotion)
        : m_value((promotion << (2 * Square::size() + Flag::size())) | (flag << (2 * Square::size())) |
                  (to << Square::size()) | from) {};

    [[nodiscard]] constexpr Square from() const { return m_value & Square::mask(); }

    [[nodiscard]] constexpr Square to() const { return (m_value >> Square::size()) & Square::mask(); }

    [[nodiscard]] constexpr Flag flag() const { return (m_value >> (2 * Square::size())) & Flag::mask(); }

    [[nodiscard]] constexpr Promotion promotion() const { return m_value >> (2 * Square::size() + Flag::size()); }

    constexpr operator uint16_t() const { return m_value; }

   private:
    uint16_t m_value;
};

// using Delta = uint32_t;
/*
constexpr Delta createDelta(Piece captured, Castling castling, uint8_t enpassant, uint8_t halfmoves,
uint8_t extraFlags = 0) {
    return static_cast<Delta>((captured & 0xF) | ((castling & 0xF) << 4) | ((enpassant & 0x3F) << 8) |
    ((halfmoves & 0xFF) << 14) | ((extraFlags & 0x3) << 22));
    }

constexpr Piece    getCaptured(Delta delta) { return Piece(delta & 0xF); }
constexpr Castling getCastling(Delta delta) { return Castling((delta >> 4) & 0xF); }
constexpr uint8_t  getEnpassant(Delta delta) { return (delta >> 8) & 0x3F; }
constexpr uint8_t  getHalfmoves(Delta delta) { return (delta >> 14) & 0xFF; }
constexpr uint8_t  getExtraFlags(Delta delta) { return (delta >> 22) & 0x3; }
*/
#endif