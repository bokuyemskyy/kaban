
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
    static constexpr Direction N{Direction::Values::N};
    static constexpr Direction S{Direction::Values::S};
    static constexpr Direction E{Direction::Values::E};
    static constexpr Direction W{Direction::Values::W};

    static constexpr Direction NN{Direction::Values::NN};
    static constexpr Direction SS{Direction::Values::SS};

    static constexpr Direction NE{Direction::Values::NE};
    static constexpr Direction SE{Direction::Values::SE};
    static constexpr Direction NW{Direction::Values::NW};
    static constexpr Direction SW{Direction::Values::SW};

    static constexpr Direction NNE{Direction::Values::NNE};
    static constexpr Direction ENE{Direction::Values::ENE};
    static constexpr Direction ESE{Direction::Values::ESE};
    static constexpr Direction SSE{Direction::Values::SSE};
    static constexpr Direction SSW{Direction::Values::SSW};
    static constexpr Direction WSW{Direction::Values::WSW};
    static constexpr Direction WNW{Direction::Values::WNW};
    static constexpr Direction NNW{Direction::Values::NNW};

    static constexpr std::array<Direction, Direction::count()> all() {
        return {N, S, E, W, NN, SS, NE, SE, NW, SW, NNE, ENE, ESE, SSE, SSW, WSW, WNW, NNW};
    }
};
