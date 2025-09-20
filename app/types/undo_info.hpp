#pragma once

#include <cstdint>

#include "castling.hpp"
#include "en_passant.hpp"
#include "halfmove.hpp"
#include "piece.hpp"
#include "strong_value.hpp"

struct UndoInfo : public StrongValue<UndoInfo, uint32_t> {
    using StrongValue::StrongValue;

    constexpr UndoInfo(Castling castling, EnPassant en_passant, Halfmove halfmove)
        : StrongValue(static_cast<uint32_t>(castling.value()) << CASTLING_SHIFT |
                      static_cast<uint32_t>(en_passant.value()) << EN_PASSANT_SHIFT |
                      static_cast<uint32_t>(halfmove.value()) << HALFMOVE_SHIFT) {}

    constexpr Castling  castling() const { return Castling((m_value >> CASTLING_SHIFT) & Castling::mask()); }
    constexpr EnPassant enPassant() const { return EnPassant((m_value >> EN_PASSANT_SHIFT) & EnPassant::mask()); }
    constexpr Halfmove  halfmove() const { return Halfmove((m_value >> HALFMOVE_SHIFT) & Halfmove::mask()); }
    constexpr Piece     captured() const { return Piece((m_value >> CAPTURED_SHIFT) & Piece::mask()); }

    constexpr void setCaptured(Piece captured) {
        m_value |= static_cast<ValueType>(captured.value() << CAPTURED_SHIFT);
    }

   private:
    static constexpr int CASTLING_SHIFT   = 0;
    static constexpr int EN_PASSANT_SHIFT = CASTLING_SHIFT + Castling::width();
    static constexpr int HALFMOVE_SHIFT   = EN_PASSANT_SHIFT + EnPassant::width();
    static constexpr int CAPTURED_SHIFT   = HALFMOVE_SHIFT + Halfmove::width();
};
