#pragma once

#include <cstdint>

#include "file.hpp"
#include "strong_value.hpp"

struct EnPassant : public StrongValue<EnPassant, uint8_t, 4> {
    using StrongValue::StrongValue;

    [[nodiscard]] constexpr bool hasValue() const { return (m_value & (1 << 3)) != 0; }

    constexpr void set(File file) noexcept { m_value = ((1 << 3) | file.value()); }
    constexpr void clear() noexcept { m_value = 0; }

    [[nodiscard]] constexpr File file() const noexcept { return File(m_value & File::mask()); }
};
