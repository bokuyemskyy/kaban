#include "movegen.hpp"

template <>
constexpr Bitboard Movegen::attackBB<PieceType::KNIGHT>(Square square) {
    constexpr std::array<Direction, 8> knightDirections = {Directions::NNE, Directions::NNW, Directions::ENE,
                                                           Directions::WNW, Directions::SSE, Directions::SSW,
                                                           Directions::ESE, Directions::WSW};

    static constexpr auto table = simpleAttackTable(knightDirections);
    return table[square.value()];
}
template <>
constexpr Bitboard Movegen::attackBB<PieceType::KING>(Square square) {
    constexpr std::array<Direction, 8> kingDirections = {Directions::E,  Directions::N,  Directions::W,
                                                         Directions::S,  Directions::NE, Directions::NW,
                                                         Directions::SE, Directions::SW};

    static constexpr std::array<Bitboard, Square::number()> table = simpleAttackTable(kingDirections);

    return table[square.value()];
}
