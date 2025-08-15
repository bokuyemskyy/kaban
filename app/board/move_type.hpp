#pragma once

#include <array>
#include <cstdint>

#include "strong.hpp"

class MoveType : public Strong::Value<MoveType, uint8_t>,
                 public Strong::Enumerator<MoveType, uint8_t, 4>,
                 public Strong::Field<MoveType, uint8_t, 2> {
   public:
    using Value::Value;

   private:
    // clang-format off
    enum Values : uint8_t {
        USUAL = 0,
        PROMOTION,
        DOUBLE_PUSH,
        CASTLING
    };
    // clang-format on

    friend struct MoveTypes;
};

struct MoveTypes {
    // clang-format off
    #define M(name) \
        static constexpr MoveType name { MoveType::Values::name }

        M(USUAL); M(PROMOTION); M(DOUBLE_PUSH); M(CASTLING);
    #undef M

    static constexpr std::array<MoveType, MoveType::number()> all() {
        return {USUAL, PROMOTION, DOUBLE_PUSH, CASTLING};
    }
    // clang-format on
};
