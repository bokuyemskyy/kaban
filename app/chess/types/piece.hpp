#pragma once

#include <array>
#include <cctype>
#include <cstdint>
#include <functional>

#include "color.hpp"
#include "piece_type.hpp"
#include "strong_value.hpp"

struct Piece : public StrongValue<Piece, uint8_t, 6> {
    using StrongValue::StrongValue;

    [[nodiscard]] constexpr bool hasValue() const { return value() < 16; }

    static constexpr ValueType count() noexcept { return static_cast<ValueType>(12); }

    explicit constexpr Piece(Color color, PieceType pieceType) noexcept
        : StrongValue(static_cast<uint8_t>((color.value() << PieceType::width()) | pieceType.value())) {}

    [[nodiscard]] constexpr Color color() const noexcept { return Color(m_value >> PieceType::width()); }

    [[nodiscard]] constexpr PieceType type() const noexcept { return PieceType(m_value & PieceType::mask()); }

    static constexpr Piece fromChar(char c) noexcept {
        Color     _color     = isupper(static_cast<unsigned char>(c)) ? Colors::WHITE : Colors::BLACK;
        PieceType piece_type = PieceType::fromChar(c);
        return Piece(_color, piece_type);
    }

    [[nodiscard]] constexpr char toChar() const noexcept {
        return color() == Colors::WHITE ? toupper(type().toChar()) : type().toChar();
    }
};

namespace Pieces {
inline constexpr Piece NONE{16};

inline constexpr Piece W_PAWN{Colors::WHITE, PieceTypes::PAWN};
inline constexpr Piece W_KNIGHT{Colors::WHITE, PieceTypes::KNIGHT};
inline constexpr Piece W_BISHOP{Colors::WHITE, PieceTypes::BISHOP};
inline constexpr Piece W_ROOK{Colors::WHITE, PieceTypes::ROOK};
inline constexpr Piece W_QUEEN{Colors::WHITE, PieceTypes::QUEEN};
inline constexpr Piece W_KING{Colors::WHITE, PieceTypes::KING};

inline constexpr Piece B_PAWN{Colors::BLACK, PieceTypes::PAWN};
inline constexpr Piece B_KNIGHT{Colors::BLACK, PieceTypes::KNIGHT};
inline constexpr Piece B_BISHOP{Colors::BLACK, PieceTypes::BISHOP};
inline constexpr Piece B_ROOK{Colors::BLACK, PieceTypes::ROOK};
inline constexpr Piece B_QUEEN{Colors::BLACK, PieceTypes::QUEEN};
inline constexpr Piece B_KING{Colors::BLACK, PieceTypes::KING};

constexpr std::array<Piece, Piece::count()> all() {
    return {W_PAWN, W_KNIGHT, W_BISHOP, W_ROOK, W_QUEEN, W_KING, B_PAWN, B_KNIGHT, B_BISHOP, B_ROOK, B_QUEEN, B_KING};
}
};  // namespace Pieces

namespace std {
template <>
struct hash<Piece> {
    std::size_t operator()(const Piece& piece) const noexcept { return std::hash<uint8_t>()(piece.value()); }
};
}  // namespace std
