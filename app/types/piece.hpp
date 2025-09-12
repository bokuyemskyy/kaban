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

    static constexpr ValueType   count() noexcept { return static_cast<ValueType>(12); }
    [[nodiscard]] constexpr bool has_value() const { return value() < 16; }
    static constexpr uint8_t     bitlen() { return 6; }
    static constexpr ValueType   bitmask() { return static_cast<ValueType>((ValueType(1) << bitlen()) - 1); }

    explicit constexpr Piece(Color color, PieceType pieceType) noexcept
        : StrongValue(static_cast<uint8_t>((color.value() << PieceType::bitlen()) | pieceType.value())) {}

    [[nodiscard]] constexpr Color color() const noexcept { return Color(m_value >> PieceType::bitlen()); }

    [[nodiscard]] constexpr PieceType piece_type() const noexcept { return PieceType(m_value & PieceType::bitmask()); }

    static constexpr Piece from_char(char c) noexcept {
        Color     color      = isupper(static_cast<unsigned char>(c)) ? Colors::WHITE : Colors::BLACK;
        PieceType piece_type = PieceType::from_char(c);
        return Piece(color, piece_type);
    }

    [[nodiscard]] constexpr char to_char() const noexcept {
        return color() == Colors::WHITE ? toupper(piece_type().to_char()) : piece_type().to_char();
    }
};

struct Pieces {
    // clang-format off
    static constexpr Piece NONE = Piece(16);

    static constexpr Piece W_PAWN = Piece(Colors::WHITE, PieceTypes::PAWN);
    static constexpr Piece W_KNIGHT = Piece(Colors::WHITE, PieceTypes::KNIGHT);
    static constexpr Piece W_BISHOP = Piece(Colors::WHITE, PieceTypes::BISHOP);
    static constexpr Piece W_ROOK = Piece(Colors::WHITE, PieceTypes::ROOK);
    static constexpr Piece W_QUEEN = Piece(Colors::WHITE, PieceTypes::QUEEN);
    static constexpr Piece W_KING = Piece(Colors::WHITE, PieceTypes::KING);

    static constexpr Piece B_PAWN = Piece(Colors::BLACK, PieceTypes::PAWN);
    static constexpr Piece B_KNIGHT = Piece(Colors::BLACK, PieceTypes::KNIGHT);
    static constexpr Piece B_BISHOP = Piece(Colors::BLACK, PieceTypes::BISHOP);
    static constexpr Piece B_ROOK = Piece(Colors::BLACK, PieceTypes::ROOK);
    static constexpr Piece B_QUEEN = Piece(Colors::BLACK, PieceTypes::QUEEN);
    static constexpr Piece B_KING = Piece(Colors::BLACK, PieceTypes::KING);

    static constexpr std::array<Piece, Piece::count()> all() {
        return {W_PAWN, W_KNIGHT, W_BISHOP, W_ROOK, W_QUEEN, W_KING,
                B_PAWN, B_KNIGHT, B_BISHOP, B_ROOK, B_QUEEN, B_KING};
    }
    // clang-format on
};

namespace std {
template <>
struct hash<Piece> {
    std::size_t operator()(const Piece& piece) const noexcept { return std::hash<uint8_t>()(piece.value()); }
};
}  // namespace std
