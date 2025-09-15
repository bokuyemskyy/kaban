#pragma once

#include <cstdint>

#include "castling.hpp"
#include "en_passant.hpp"
#include "halfmove.hpp"
#include "piece.hpp"
#include "strong_value.hpp"

class UndoInfo : public StrongValue<UndoInfo, uint32_t> {
   public:
    using StrongValue::StrongValue;

    constexpr UndoInfo(Castling castling, EnPassant en_passant, Halfmove halfmove)
        : StrongValue(static_cast<uint32_t>(castling.value()) << CASTLING_SHIFT |
                      static_cast<uint32_t>(en_passant.value()) << EN_PASSANT_SHIFT |
                      static_cast<uint32_t>(halfmove.value()) << HALFMOVE_SHIFT) {}

    [[nodiscard]] constexpr Castling castling() const { return Castling(m_value >> CASTLING_SHIFT); }

    [[nodiscard]] constexpr EnPassant enPassant() const { return EnPassant(m_value >> EN_PASSANT_SHIFT); }

    [[nodiscard]] constexpr Halfmove halfmove() const { return Halfmove(m_value >> HALFMOVE_SHIFT); }

    [[nodiscard]] constexpr Piece captured() const { return Piece(m_value >> CAPTURED_SHIFT); }

    constexpr void setCaptured(Piece captured) { m_value |= (captured.value() << CAPTURED_SHIFT); }

   private:
    static constexpr int CASTLING_SHIFT   = 0;
    static constexpr int EN_PASSANT_SHIFT = CASTLING_SHIFT + Castling::bitlength();
    static constexpr int HALFMOVE_SHIFT   = EN_PASSANT_SHIFT + EnPassant::bitlength();
    static constexpr int CAPTURED_SHIFT   = HALFMOVE_SHIFT + Halfmove::bitlength();
};
