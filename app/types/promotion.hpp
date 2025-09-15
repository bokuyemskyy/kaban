#pragma once

#include <array>
#include <cstdint>

#include "strong_value.hpp"

class Promotion : public StrongValue<Promotion, uint8_t> {
   public:
    using StrongValue::StrongValue;

    static constexpr ValueType count() noexcept { return static_cast<ValueType>(5); }
    static constexpr uint8_t   bitlength() { return 3; }
    static constexpr ValueType bitmask() { return static_cast<ValueType>((ValueType(1) << bitlength()) - 1); }

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

    static constexpr std::array<Promotion, Promotion::count()> all() { 
        return {NONE, TO_KNIGHT, TO_BISHOP, TO_ROOK, TO_QUEEN}; 
    }
    // clang-format on
};
