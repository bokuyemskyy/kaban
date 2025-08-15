#pragma once

#include <array>
#include <cctype>
#include <cstdint>

#include "strong.hpp"
class PieceType : public Strong::Value<PieceType, uint8_t>,
                  public Strong::Enumerator<PieceType, uint8_t, 6>,
                  public Strong::Field<PieceType, uint8_t, 3> {
   public:
    using Value::Value;

    static constexpr PieceType fromChar(char c) {
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        for (uint8_t i = 0; i < number(); ++i)
            if (chars[i] == c) return PieceType(i);
        return PieceType();
    }
    [[nodiscard]] constexpr char toChar() const { return chars[m_value]; }

   private:
    static constexpr std::array<char, 6> chars = {'p', 'n', 'b', 'r', 'q', 'k'};

    enum Values : uint8_t {
        PAWN   = 0b000,
        KNIGHT = 0b001,
        BISHOP = 0b010,
        ROOK   = 0b011,
        QUEEN  = 0b100,
        KING   = 0b101,
    };

    friend struct PieceTypes;
};
struct PieceTypes {
    static constexpr PieceType PAWN{PieceType::Values::PAWN};
    static constexpr PieceType KNIGHT{PieceType::Values::KNIGHT};
    static constexpr PieceType BISHOP{PieceType::Values::BISHOP};
    static constexpr PieceType ROOK{PieceType::Values::ROOK};
    static constexpr PieceType QUEEN{PieceType::Values::QUEEN};
    static constexpr PieceType KING{PieceType::Values::KING};

    static constexpr std::array<PieceType, PieceType::number()> all() {
        return {PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING};
    }
};