
#pragma once

#include <array>
#include <cstdint>

#include "strong_value.hpp"

class Color : public StrongValue<Color, uint8_t> {
   public:
    using StrongValue::StrongValue;

    static constexpr ValueType count() noexcept { return static_cast<ValueType>(2); }
    static constexpr uint8_t   bitlen() { return 1; }
    static constexpr ValueType bitmask() { return static_cast<ValueType>((ValueType(1) << bitlen()) - 1); }

    [[nodiscard]] constexpr Color operator!() const noexcept { return Color(static_cast<uint8_t>(m_value ^ 1)); }

    constexpr Color& flip() noexcept {
        *this = !*this;
        return *this;
    }
};

struct Colors {
    static constexpr Color WHITE{0};
    static constexpr Color BLACK{1};

    static constexpr std::array<Color, Color::count()> all() { return {WHITE, BLACK}; }
};