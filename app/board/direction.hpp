#pragma once

#include <cstdint>
struct Direction {
   public:
    enum : int8_t {
        E = 1,
        N = 8,
        W = -1,
        S = -8,

        NN = N + N,
        SS = S + S,

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

    constexpr Direction(int8_t value) : m_value(value) {};

    [[nodiscard]] constexpr int8_t value() const { return m_value; }

   private:
    int8_t m_value;
};

/*
constexpr std::array<Direction, 1> whitePawnPushDirections = {Direction::N};

constexpr std::array<Direction, 2> whitePawnAttackDirections = {Direction::NE, Direction::NW};

constexpr std::array<Direction, 8> queenDirections = {Direction::NE, Direction::NW, Direction::SE, Direction::SW,
                                                      Direction::N,  Direction::S,  Direction::E,  Direction::W};
*/