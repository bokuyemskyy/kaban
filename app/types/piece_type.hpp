#pragma once

#include <array>
#include <cassert>
#include <cctype>
#include <cstdint>
#include <span>

#include "color.hpp"
#include "direction.hpp"
#include "strong_value.hpp"

class PieceType : public StrongValue<PieceType, uint8_t> {
   public:
    using StrongValue::StrongValue;

    static constexpr ValueType count() noexcept { return static_cast<ValueType>(6); }
    static constexpr uint8_t   bitlength() { return 3; }
    static constexpr ValueType bitmask() { return static_cast<ValueType>((ValueType(1) << bitlength()) - 1); }

    static constexpr PieceType fromChar(char c) {
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        for (uint8_t i = 0; i < count(); ++i)
            if (chars[i] == c) return PieceType(i);
        return PieceType();
    }
    [[nodiscard]] constexpr char toChar() const { return chars[m_value]; }

    constexpr std::span<const Direction> directions(Color color) const {
        assert(value() == PAWN);

        static constexpr std::array<Direction, 2> WHITE_PAWN_DIRECTIONS = {Directions::NW, Directions::NE};
        static constexpr std::array<Direction, 2> BLACK_PAWN_DIRECTIONS = {Directions::SE, Directions::SW};

        return color == Colors::WHITE ? WHITE_PAWN_DIRECTIONS : BLACK_PAWN_DIRECTIONS;
    }

    constexpr std::span<const Direction> directions() const {
        assert(value() != PAWN);

        static constexpr std::array<Direction, 8> KNIGHT_DIRECTIONS = {
            Directions::NNE, Directions::NNW, Directions::ENE, Directions::WNW,
            Directions::SSE, Directions::SSW, Directions::ESE, Directions::WSW};
        static constexpr std::array<Direction, 4> BISHOP_DIRECTIONS = {Directions::NE, Directions::NW, Directions::SE,
                                                                       Directions::SW};
        static constexpr std::array<Direction, 4> ROOK_DIRECTIONS   = {Directions::N, Directions::S, Directions::E,
                                                                       Directions::W};
        static constexpr std::array<Direction, 8> QUEEN_DIRECTIONS  = {Directions::N,  Directions::S,  Directions::E,
                                                                       Directions::W,  Directions::NE, Directions::NW,
                                                                       Directions::SE, Directions::SW};
        static constexpr std::array<Direction, 8> KING_DIRECTIONS   = {Directions::N,  Directions::S,  Directions::E,
                                                                       Directions::W,  Directions::NE, Directions::NW,
                                                                       Directions::SE, Directions::SW};

        switch (value()) {
            case KNIGHT:
                return KNIGHT_DIRECTIONS;
            case BISHOP:
                return BISHOP_DIRECTIONS;
            case ROOK:
                return ROOK_DIRECTIONS;
            case QUEEN:
                return QUEEN_DIRECTIONS;
            case KING:
            default:
                return KING_DIRECTIONS;
        }
    }

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

    static constexpr std::array<PieceType, PieceType::count()> all() {
        return {PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING};
    }
};