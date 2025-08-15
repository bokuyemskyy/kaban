#pragma once

#include <array>
#include <cctype>
#include <cstdint>
#include <functional>

#include "color.hpp"
#include "piece_type.hpp"
#include "strong.hpp"

class Piece : public Strong::Value<Piece, uint8_t>,
              public Strong::Enumerator<Piece, uint8_t, 12>,
              public Strong::Field<Piece, uint8_t, 6> {
   public:
    using Value::Value;

    explicit constexpr Piece(Color color, PieceType pieceType) noexcept
        : Value(static_cast<uint8_t>((color.value() << PieceType::size()) | pieceType.value())) {}

    [[nodiscard]] constexpr bool hasValue() const { return color().hasValue() && pieceType().hasValue(); }

    [[nodiscard]] constexpr Color color() const noexcept { return Color(m_value >> PieceType::size()); }

    [[nodiscard]] constexpr PieceType pieceType() const noexcept { return PieceType(m_value & PieceType::mask()); }

    static constexpr Piece fromChar(char c) noexcept {
        Color     color      = isupper(static_cast<unsigned char>(c)) ? Colors::WHITE : Colors::BLACK;
        PieceType piece_type = PieceType::fromChar(c);
        return Piece(color, piece_type);
    }

    [[nodiscard]] constexpr char toChar() const noexcept {
        return color() == Colors::WHITE ? toupper(pieceType().toChar()) : pieceType().toChar();
    }

   private:
    // clang-format off
    enum Values : uint8_t {
        NONE = 16
    };
    // clang-format on

    friend struct Pieces;
};

struct Pieces {
    // clang-format off
    static constexpr Piece NONE = Piece(Piece::Values::NONE);

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

    static constexpr std::array<Piece, Piece::number()> all() {
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
