#include "movegen.hpp"

template <>
constexpr Bitboard Movegen::attackBB<PieceType::KNIGHT>(Square square) {
    constexpr std::array<Direction, 8> knightDirections = {Direction::NNE, Direction::NNW, Direction::ENE,
                                                           Direction::WNW, Direction::SSE, Direction::SSW,
                                                           Direction::ESE, Direction::WSW};

    static constexpr auto table = simpleAttackTable(knightDirections);
    return table[square.value()];
}
template <>
constexpr Bitboard Movegen::attackBB<PieceType::KING>(Square square) {
    constexpr std::array<Direction, 8> kingDirections = {Direction::E,  Direction::N,  Direction::W,  Direction::S,
                                                         Direction::NE, Direction::NW, Direction::SE, Direction::SW};

    static constexpr auto table = simpleAttackTable(kingDirections);
    return table[square.value()];
}
