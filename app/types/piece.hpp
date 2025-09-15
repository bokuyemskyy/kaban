#pragma once

#include <array>
#include <cctype>
#include <cstdint>
#include <functional>

#include "color.hpp"
#include "piece_type.hpp"
#include "strong_value.hpp"

class Piece : public StrongValue<Piece, uint8_t> {
   public:
    using StrongValue::StrongValue;

    [[nodiscard]] constexpr bool hasValue() const { return value() < 16; }

    static constexpr ValueType count() noexcept { return static_cast<ValueType>(12); }
    static constexpr uint8_t   bitlength() { return 6; }
    static constexpr ValueType bitmask() { return static_cast<ValueType>((ValueType(1) << bitlength()) - 1); }

    explicit constexpr Piece(Color color, PieceType pieceType) noexcept
        : StrongValue(static_cast<uint8_t>((color.value() << PieceType::bitlength()) | pieceType.value())) {}

    [[nodiscard]] constexpr Color color() const noexcept { return Color(m_value >> PieceType::bitlength()); }

    [[nodiscard]] constexpr PieceType pieceType() const noexcept { return PieceType(m_value & PieceType::bitmask()); }

    static constexpr Piece fromChar(char c) noexcept {
        Color     color      = isupper(static_cast<unsigned char>(c)) ? Colors::WHITE : Colors::BLACK;
        PieceType piece_type = PieceType::fromChar(c);
        return Piece(color, piece_type);
    }

    [[nodiscard]] constexpr char toChar() const noexcept {
        return color() == Colors::WHITE ? toupper(pieceType().toChar()) : pieceType().toChar();
    }
};

struct Pieces {
    static constexpr Piece NONE{16};

    static constexpr Piece W_PAWN{Colors::WHITE, PieceTypes::PAWN};
    static constexpr Piece W_KNIGHT{Colors::WHITE, PieceTypes::KNIGHT};
    static constexpr Piece W_BISHOP{Colors::WHITE, PieceTypes::BISHOP};
    static constexpr Piece W_ROOK{Colors::WHITE, PieceTypes::ROOK};
    static constexpr Piece W_QUEEN{Colors::WHITE, PieceTypes::QUEEN};
    static constexpr Piece W_KING{Colors::WHITE, PieceTypes::KING};

    static constexpr Piece B_PAWN{Colors::BLACK, PieceTypes::PAWN};
    static constexpr Piece B_KNIGHT{Colors::BLACK, PieceTypes::KNIGHT};
    static constexpr Piece B_BISHOP{Colors::BLACK, PieceTypes::BISHOP};
    static constexpr Piece B_ROOK{Colors::BLACK, PieceTypes::ROOK};
    static constexpr Piece B_QUEEN{Colors::BLACK, PieceTypes::QUEEN};
    static constexpr Piece B_KING{Colors::BLACK, PieceTypes::KING};

    static constexpr std::array<Piece, Piece::count()> all() {
        return {W_PAWN, W_KNIGHT, W_BISHOP, W_ROOK, W_QUEEN, W_KING,
                B_PAWN, B_KNIGHT, B_BISHOP, B_ROOK, B_QUEEN, B_KING};
    }
};

namespace std {
template <>
struct hash<Piece> {
    std::size_t operator()(const Piece& piece) const noexcept { return std::hash<uint8_t>()(piece.value()); }
};
}  // namespace std
