#pragma once

#include <cstdint>

#include "castling.hpp"
#include "en_passant.hpp"
#include "piece.hpp"

struct State {
   public:
    constexpr State(Castling castling, EnPassant en_passant, uint8_t halfmoves)
        : m_value(static_cast<uint32_t>(castling.value() & Castling::mask()) << CASTLING_SHIFT |
                  static_cast<uint32_t>(en_passant.value() & EnPassant::mask()) << EN_PASSANT_SHIFT |
                  static_cast<uint32_t>(halfmoves & HALFMOVES_MASK) << HALFMOVES_SHIFT) {}

    [[nodiscard]] constexpr Castling castling() const { return (m_value >> CASTLING_SHIFT) & Castling::mask(); }

    [[nodiscard]] constexpr EnPassant en_passant() const { return (m_value >> EN_PASSANT_SHIFT) & EnPassant::mask(); }

    [[nodiscard]] constexpr uint8_t halfmoves() const { return (m_value >> HALFMOVES_SHIFT) & HALFMOVES_MASK; }

    constexpr void setCaptured(Piece captured) { m_value |= ((captured.value() & Piece::mask()) << CAPTURED_SHIFT); }
    [[nodiscard]] constexpr Piece captured() const { return (m_value >> CAPTURED_SHIFT) & Piece::mask(); }

   private:
    uint16_t m_value;

    static constexpr int HALFMOVES_BITS = 6;

    static constexpr int CASTLING_SHIFT   = 0;
    static constexpr int EN_PASSANT_SHIFT = CASTLING_SHIFT + Castling::size();
    static constexpr int HALFMOVES_SHIFT  = EN_PASSANT_SHIFT + EnPassant::size();
    static constexpr int CAPTURED_SHIFT   = HALFMOVES_SHIFT + HALFMOVES_BITS;

    static constexpr uint32_t HALFMOVES_MASK = 0b111111;
};
