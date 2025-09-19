
#pragma once

#include <array>
#include <cstdint>
#include <span>

#include "piece.hpp"
#include "piece_type.hpp"
#include "strong_value.hpp"

struct Direction : public StrongValue<Direction, int8_t> {
    using StrongValue::StrongValue;

    [[nodiscard]] constexpr Direction operator+(const Direction& other) const {
        return Direction(m_value + other.value());
    }
    [[nodiscard]] constexpr Direction operator-(const Direction& other) const {
        return Direction(m_value - other.value());
    }

    // needs optimization
    [[nodiscard]] constexpr int8_t horizontal() const noexcept {
        int8_t h = m_value % 8;
        if (h > 3) h -= 8;
        if (h < -3) h += 8;
        return h;
    }

    // needs optimization
    [[nodiscard]] constexpr int8_t vertical() const noexcept {
        switch (m_value) {
            case 8:
            case 7:
            case 9:
                return 1;
            case 16:
            case 15:
            case 17:
                return 2;
            case -8:
            case -9:
            case -7:
                return -1;
            case -16:
            case -17:
            case -15:
                return -2;
            case 1:
            case -1:
                return 0;
            case 10:
            case -6:
            case 6:
            case -10:
                return 1;
            default:
                return 0;
        }
    }
};

namespace Directions {
inline constexpr Direction N{8};
inline constexpr Direction S{-8};
inline constexpr Direction E{1};
inline constexpr Direction W{-1};

inline constexpr Direction NN{N + N};
inline constexpr Direction SS{S + S};

inline constexpr Direction NE{N + E};
inline constexpr Direction SE{S + E};
inline constexpr Direction NW{N + W};
inline constexpr Direction SW{S + W};

inline constexpr Direction NNE{N + N + E};
inline constexpr Direction ENE{E + N + E};
inline constexpr Direction ESE{E + S + E};
inline constexpr Direction SSE{S + S + E};
inline constexpr Direction SSW{S + S + W};
inline constexpr Direction WSW{W + S + W};
inline constexpr Direction WNW{W + N + W};
inline constexpr Direction NNW{N + N + W};

constexpr uint8_t count() noexcept { return 18; }

constexpr std::array<Direction, count()> all() {
    return {N, S, E, W, NN, SS, NE, SE, NW, SW, NNE, ENE, ESE, SSE, SSW, WSW, WNW, NNW};
}

constexpr const std::array<Direction, 2>& rank() {
    static constexpr std::array<Direction, 2> directions = {E, W};
    return directions;
}
constexpr const std::array<Direction, 2>& file() {
    static constexpr std::array<Direction, 2> directions = {N, S};
    return directions;
}
constexpr const std::array<Direction, 2>& diag() {
    static constexpr std::array<Direction, 2> directions = {NE, SW};
    return directions;
}
constexpr const std::array<Direction, 2>& antiDiag() {
    static constexpr std::array<Direction, 2> directions = {NW, SE};
    return directions;
}

constexpr const std::array<Direction, 2>& whitePawn() {
    static constexpr std::array<Direction, 2> directions = {NW, NE};
    return directions;
}

constexpr const std::array<Direction, 2>& blackPawn() {
    static constexpr std::array<Direction, 2> directions = {SE, SW};
    return directions;
}

constexpr const std::array<Direction, 8>& knight() {
    static constexpr std::array<Direction, 8> directions = {NNE, NNW, ENE, WNW, SSE, SSW, ESE, WSW};
    return directions;
}

constexpr const std::array<Direction, 4>& bishop() {
    static constexpr std::array<Direction, 4> directions = {NE, NW, SE, SW};
    return directions;
}

constexpr const std::array<Direction, 4>& rook() {
    static constexpr std::array<Direction, 4> directions = {N, S, E, W};
    return directions;
}

constexpr const std::array<Direction, 8>& queen() {
    static constexpr std::array<Direction, 8> directions = {N, S, E, W, NE, NW, SE, SW};
    return directions;
}

constexpr const std::array<Direction, 8>& king() {
    static constexpr std::array<Direction, 8> directions = queen();  // same as queen
    return directions;
}

[[nodiscard]] constexpr std::span<const Direction> of(Piece piece) {
    assert(piece.type() == PieceTypes::PAWN);
    return (piece == Pieces::W_PAWN) ? whitePawn() : blackPawn();
}

[[nodiscard]] constexpr std::span<const Direction> of(PieceType piece_type) {
    assert(piece_type != PieceTypes::PAWN);
    switch (piece_type.value()) {
        case PieceTypes::KNIGHT.value():
            return knight();
        case PieceTypes::BISHOP.value():
            return bishop();
        case PieceTypes::ROOK.value():
            return rook();
        case PieceTypes::QUEEN.value():
            return queen();
        case PieceTypes::KING.value():
        default:
            return king();
    }
}
};  // namespace Directions
