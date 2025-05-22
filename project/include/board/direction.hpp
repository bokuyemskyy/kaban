#ifndef DIRECTION_HPP
#define DIRECTION_HPP

#include <array>
#include <cstdint>

#include "piece.hpp"

using Direction = int8_t;
namespace Directions {
enum value : int8_t {
    E = 1,
    N = 8,
    W = -1,
    S = -8,

    NE = N + E,
    SE = S + E,
    NW = N + W,
    SW = S + W,

    NNE = N + N + E,
    ENE = E + E + N,
    ESE = E + E + S,
    SSE = S + S + E,
    SSW = S + S + W,
    WSW = W + W + S,
    WNW = W + W + N,
    NNW = N + N + W,

    NONE = 0
};
}  // namespace Directions

constexpr int PAWN_DIRECTIONS_NB        = 1;
constexpr int PAWN_ATTACK_DIRECTIONS_NB = 2;
constexpr int KNIGHT_DIRECTIONS_NB      = 8;
constexpr int BISHOP_DIRECTIONS_NB      = 4;
constexpr int ROOK_DIRECTIONS_NB        = 4;
constexpr int QUEEN_DIRECTIONS_NB       = 8;
constexpr int KING_DIRECTIONS_NB        = 8;

constexpr std::array<Direction, PAWN_DIRECTIONS_NB> whitePawnPushDirections = {Directions::N};

constexpr std::array<Direction, PAWN_ATTACK_DIRECTIONS_NB> whitePawnAttackDirections = {Directions::NE, Directions::NW};

constexpr std::array<Direction, KNIGHT_DIRECTIONS_NB> knightDirections = {
    Directions::NNE, Directions::NNW, Directions::ENE, Directions::WNW,
    Directions::SSE, Directions::SSW, Directions::ESE, Directions::WSW};

constexpr std::array<Direction, BISHOP_DIRECTIONS_NB> bishopDirections = {Directions::NE, Directions::NW,
                                                                          Directions::SE, Directions::SW};

constexpr std::array<Direction, ROOK_DIRECTIONS_NB> rookDirections = {Directions::N, Directions::S, Directions::E,
                                                                      Directions::W};

constexpr std::array<Direction, QUEEN_DIRECTIONS_NB> queenDirections = {Directions::NE, Directions::NW, Directions::SE,
                                                                        Directions::SW, Directions::N,  Directions::S,
                                                                        Directions::E,  Directions::W};

constexpr std::array<Direction, KING_DIRECTIONS_NB> kingDirections = {Directions::E,  Directions::N,  Directions::W,
                                                                      Directions::S,  Directions::NE, Directions::NW,
                                                                      Directions::SE, Directions::SW};

#endif