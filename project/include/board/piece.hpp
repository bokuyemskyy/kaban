#ifndef PIECE_HPP
#define PIECE_HPP

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <string_view>

using PieceType = uint8_t;
namespace PieceTypes {
enum value : PieceType {
    PAWN   = 0b000,
    KNIGHT = 0b001,
    BISHOP = 0b010,
    ROOK   = 0b011,
    QUEEN  = 0b100,
    KING   = 0b101,

    FIRST = PAWN,
    LAST  = KING,

    NONE = 0b111,
    MASK = 0b111,
    SIZE = 3,
    NB   = 6
};
}  // namespace PieceTypes

using Color = uint8_t;
namespace Colors {
enum value : uint8_t {
    WHITE = 0,
    BLACK = 1,

    MASK = 0b1,
    SIZE = 1,
    NB   = 2
};
}  // namespace Colors

using Piece = uint8_t;
namespace Pieces {
enum value : uint8_t {
    WPAWN   = static_cast<uint8_t>(Colors::WHITE) | static_cast<uint8_t>(PieceTypes::PAWN),
    WKNIGHT = static_cast<uint8_t>(Colors::WHITE) | static_cast<uint8_t>(PieceTypes::KNIGHT),
    WBISHOP = static_cast<uint8_t>(Colors::WHITE) | static_cast<uint8_t>(PieceTypes::BISHOP),
    WROOK   = static_cast<uint8_t>(Colors::WHITE) | static_cast<uint8_t>(PieceTypes::ROOK),
    WQUEEN  = static_cast<uint8_t>(Colors::WHITE) | static_cast<uint8_t>(PieceTypes::QUEEN),
    WKING   = static_cast<uint8_t>(Colors::WHITE) | static_cast<uint8_t>(PieceTypes::KING),

    BPAWN   = static_cast<uint8_t>(Colors::BLACK) | static_cast<uint8_t>(PieceTypes::PAWN),
    BKNIGHT = static_cast<uint8_t>(Colors::BLACK) | static_cast<uint8_t>(PieceTypes::KNIGHT),
    BBISHOP = static_cast<uint8_t>(Colors::BLACK) | static_cast<uint8_t>(PieceTypes::BISHOP),
    BROOK   = static_cast<uint8_t>(Colors::BLACK) | static_cast<uint8_t>(PieceTypes::ROOK),
    BQUEEN  = static_cast<uint8_t>(Colors::BLACK) | static_cast<uint8_t>(PieceTypes::QUEEN),
    BKING   = static_cast<uint8_t>(Colors::BLACK) | static_cast<uint8_t>(PieceTypes::KING),

    FIRST = WPAWN,
    LAST  = BKING,

    NONE = 0b1111,
    MASK = 0b1111,
    SIZE = 4,
    NB   = 12
};
}  // namespace Pieces

// ############### Helpers ###############

// ##### Color #####

inline Color getColor(Piece piece) {
    assert(piece != Pieces::NONE);
    return (piece >> PieceTypes::SIZE) & Colors::MASK;
}

// ##### Piece #####

inline Piece createPiece(Color color, PieceType pieceType) {
    assert(color < Colors::NB);
    assert(pieceType != PieceTypes::NONE);
    return static_cast<Piece>((color << PieceTypes::SIZE) | pieceType);
}

constexpr std::string_view charPieceMap = "PNBRQK  pnbrqk?";

constexpr Piece charToPiece(char c) {
    const auto *it = std::ranges::find(charPieceMap, c);
    return it != charPieceMap.end() ? Pieces::value(it - charPieceMap.begin()) : Pieces::NONE;
}

constexpr char pieceToChar(Piece p) {
    assert(p != Pieces::NONE);
    return charPieceMap[p];
}

// ##### Piece Type #####

inline PieceType getPieceType(Piece piece) {
    assert(piece != Pieces::NONE);
    return piece & PieceTypes::MASK;
}

#endif
