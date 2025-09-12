#pragma once

#include <array>
#include <cstdint>

#include "strong_value.hpp"

class MoveType : public StrongValue<MoveType, uint8_t> {
   public:
    using StrongValue::StrongValue;

    static constexpr ValueType count() noexcept { return static_cast<ValueType>(4); }
    static constexpr uint8_t   bitlen() { return 2; }
    static constexpr ValueType bitmask() { return static_cast<ValueType>((ValueType(1) << bitlen()) - 1); }

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

    static constexpr std::array<MoveType, MoveType::count()> all() {
        return {USUAL, PROMOTION, DOUBLE_PUSH, CASTLING};
    }
    // clang-format on
};
