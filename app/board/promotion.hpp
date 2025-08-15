#pragma once

#include <array>
#include <cstdint>

#include "strong.hpp"

class Promotion : public Strong::Value<Promotion, uint8_t>,
                  public Strong::Enumerator<Promotion, uint8_t, 5>,
                  public Strong::Field<Promotion, uint8_t, 3> {
   public:
    using Value::Value;

   private:
    // clang-format off
    enum Values : uint8_t {
        NONE = 0,
        TO_KNIGHT,
        TO_BISHOP,
        TO_ROOK,
        TO_QUEEN,
    };
    // clang-format on

    friend struct Promotions;
};

struct Promotions {
    // clang-format off
    #define P(name) \
        static constexpr Promotion name { Promotion::Values::name }

        P(NONE); P(TO_KNIGHT); P(TO_BISHOP); P(TO_ROOK); P(TO_QUEEN);
    #undef P

    static constexpr std::array<Promotion, Promotion::number()> all() { 
        return {NONE, TO_KNIGHT, TO_BISHOP, TO_ROOK, TO_QUEEN}; 
    }
    // clang-format on
};
