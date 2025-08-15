#pragma once

#include <array>
#include <cstdint>

#include "strong.hpp"

class Color : public Strong::Value<Color, uint8_t>,
              public Strong::Enumerator<Color, uint8_t, 2>,
              public Strong::Field<Color, uint8_t, 1> {
   public:
    using Value::Value;

    [[nodiscard]] constexpr Color flipped() const noexcept { return Color(static_cast<uint8_t>(m_value ^ 1)); }

    constexpr Color& flip() noexcept {
        m_value ^= 1;
        return *this;
    }

   private:
    enum Values : ValueType {
        WHITE = 0,
        BLACK
    };
    friend struct Colors;
};

struct Colors {
    static constexpr Color WHITE{Color::Values::WHITE};
    static constexpr Color BLACK{Color::Values::BLACK};

    static constexpr std::array<Color, Color::number()> all() { return {WHITE, BLACK}; }
};