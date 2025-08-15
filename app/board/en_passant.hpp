#pragma once

#include <cstdint>

#include "color.hpp"
#include "file.hpp"
#include "square.hpp"
#include "strong.hpp"

class EnPassant : public Strong::Value<EnPassant, uint8_t>, public Strong::Field<EnPassant, uint8_t, 4> {
   public:
    using Value::Value;

    constexpr void set(File file) noexcept { m_value = ((1 << 3) | file.value()); }
    constexpr void clear() noexcept { m_value = 0; }

    [[nodiscard]] constexpr bool hasValue() const noexcept { return (m_value & (1 << 3)) != 0; }
    [[nodiscard]] constexpr File file() const noexcept { return File(m_value & File::mask()); }

    [[nodiscard]] Square square(Color sideToMove) const noexcept {
        return Square(file(), sideToMove == Colors::WHITE ? Ranks::R6 : Ranks::R3);
    }
};
