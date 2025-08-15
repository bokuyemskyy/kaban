#pragma once

#include <array>
#include <cstdint>

#include "file.hpp"
#include "strong.hpp"

class Direction : public Strong::Value<Direction, int8_t>, public Strong::Enumerator<Direction, int8_t, 18> {
   public:
    using Value::Value;

    [[nodiscard]] constexpr int8_t horizontal() const noexcept { return static_cast<int8_t>(m_value % File::number()); }
    [[nodiscard]] constexpr int8_t vertical() const noexcept { return static_cast<int8_t>(m_value / File::number()); }

   private:
    // clang-format off
    enum Values : int8_t {
        E   = 1,
        N   = 8,
        W   = -1,
        S   = -8,

        NN  = N + N,
        SS  = S + S,

        NE  = N + E,
        SE  = S + E,
        NW  = N + W,
        SW  = S + W,

        NNE = N + N + E,
        ENE = E + E + N,
        ESE = E + E + S,
        SSE = S + S + E,
        SSW = S + S + W,
        WSW = W + W + S,
        WNW = W + W + N,
        NNW = N + N + W,
    };
    // clang-format on

    friend struct Directions;
};

struct Directions {
    // clang-format off
    #define D(name) \
        static constexpr Direction name { Direction::Values::name }

        D(N); D(S); D(E); D(W);
        D(NN); D(SS);
        D(NE); D(SE); D(NW); D(SW);
        D(NNE); D(ENE); D(ESE); D(SSE); D(SSW); D(WSW); D(WNW); D(NNW);
    #undef D

    static constexpr std::array<Direction, Direction::number()> all() {
        return {N, S, E, W, NN, SS, NE, SE, NW, SW, NNE, ENE, ESE, SSE, SSW, WSW, WNW, NNW};
    }
    // clang-format on
};
