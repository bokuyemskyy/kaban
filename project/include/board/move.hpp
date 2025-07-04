// HAS TESTS

#ifndef MOVE_HPP
#define MOVE_HPP

#include <cstdint>

#include "castling.hpp"
#include "navigation.hpp"

using Delta = uint32_t;

/*
Move encoding:
Bits    Data
0-5	    Origin square
6-11    Destination square
12-13   Special flag (0 = usual move, 1 = promotion, 2 = double pawn push, 3 = castling)
14-15   Promotion piece (0 = knight, 1 = bishop, 2 = rook, 3 = queen) (if any)
*/

using Flag      = uint8_t;
using Promotion = uint8_t;

struct Move {
   private:
    uint16_t m_val;

    static constexpr int FROM_SHIFT  = 0;
    static constexpr int TO_SHIFT    = 6;
    static constexpr int FLAG_SHIFT  = 12;
    static constexpr int PROMO_SHIFT = 14;

    static constexpr uint16_t MASK_6BITS = 0x3F;
    static constexpr uint16_t MASK_2BITS = 0x3;

   public:
    constexpr Move(uint16_t val = 0) : m_val(val) {};

    constexpr operator uint16_t() const { return m_val; }

    [[nodiscard]] constexpr Square from() const { return Square((m_val >> FROM_SHIFT) & MASK_6BITS); }

    [[nodiscard]] constexpr Square to() const { return Square((m_val >> TO_SHIFT) & MASK_6BITS); }

    [[nodiscard]] constexpr Flag flag() const { return static_cast<Flag>((m_val >> FLAG_SHIFT) & MASK_2BITS); }

    [[nodiscard]] constexpr Promotion promotion() const {
        return static_cast<Promotion>((m_val >> PROMO_SHIFT) & MASK_2BITS);
    }

    static constexpr Move create(Square from, Square to, Flag flag = Flag(0), Promotion promo = Promotion(0)) {
        return (static_cast<uint16_t>(from) << FROM_SHIFT) | (static_cast<uint16_t>(to) << TO_SHIFT) |
               (static_cast<uint16_t>(flag) << FLAG_SHIFT) | (static_cast<uint16_t>(promo) << PROMO_SHIFT);
    }
};

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

#endif