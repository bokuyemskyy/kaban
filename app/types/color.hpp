
#pragma once

#include <array>
#include <cstdint>

#include "strong_value.hpp"

struct Color : public StrongValue<Color, uint8_t, 1> {
    using StrongValue::StrongValue;

    [[nodiscard]] constexpr Color operator!() const noexcept { return Color(static_cast<uint8_t>(m_value ^ 1)); }

    constexpr Color& flip() noexcept {
        *this = !*this;
        return *this;
    }
};

namespace Colors {
inline constexpr Color WHITE{0};
inline constexpr Color BLACK{1};

constexpr uint8_t count() noexcept { return 2; }

constexpr std::array<Color, count()> all() { return {WHITE, BLACK}; }
};  // namespace Colors