
#pragma once

#include <array>
#include <cstdint>

#include "file.hpp"
#include "strong_value.hpp"

class Direction : public StrongValue<Direction, int8_t> {
   public:
    using StrongValue::StrongValue;

    static constexpr ValueType count() noexcept { return static_cast<ValueType>(18); }

    [[nodiscard]] constexpr int8_t horizontal() const noexcept {
        switch (m_value) {
            case E:
                return 1;
            case W:
                return -1;
            case N:
            case S:
                return 0;
            case NE:
                return 1;
            case NW:
                return -1;
            case SE:
                return 1;
            case SW:
                return -1;
            case NNE:
                return 1;
            case ENE:
                return 2;
            case ESE:
                return 2;
            case SSE:
                return 1;
            case SSW:
                return -1;
            case WSW:
                return -2;
            case WNW:
                return -2;
            case NNW:
                return -1;
            case NN:
            case SS:
                return 0;
            default:
                return 0;
        }
    }

    [[nodiscard]] constexpr int8_t vertical() const noexcept {
        switch (m_value) {
            case N:
                return 1;
            case S:
                return -1;
            case E:
            case W:
                return 0;
            case NE:
                return 1;
            case NW:
                return 1;
            case SE:
                return -1;
            case SW:
                return -1;
            case NNE:
                return 2;
            case ENE:
                return 1;
            case ESE:
                return -1;
            case SSE:
                return -2;
            case SSW:
                return -2;
            case WSW:
                return -1;
            case WNW:
                return 1;
            case NNW:
                return 2;
            case NN:
                return 2;
            case SS:
                return -2;
            default:
                return 0;
        }
    }

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

    static constexpr std::array<Direction, Direction::count()> all() {
        return {N, S, E, W, NN, SS, NE, SE, NW, SW, NNE, ENE, ESE, SSE, SSW, WSW, WNW, NNW};
    }
    // clang-format on

    static constexpr std::array<Direction, 2> wpawn() { return {NW, NE}; }
    static constexpr std::array<Direction, 2> bpawn() { return {SE, SW}; }
    static constexpr std::array<Direction, 8> knight() { return {NNE, NNW, ENE, WNW, SSE, SSW, ESE, WSW}; }
    static constexpr std::array<Direction, 4> bishop() { return {NE, NW, SE, SW}; }
    static constexpr std::array<Direction, 4> rook() { return {N, S, E, W}; }
    static constexpr std::array<Direction, 8> queen() { return {N, S, E, W, NE, NW, SE, SW}; }
    static constexpr std::array<Direction, 8> king() { return {N, S, E, W, NE, NW, SE, SW}; }
};
