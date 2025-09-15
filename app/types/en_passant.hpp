#pragma once

#include <cstdint>

#include "color.hpp"
#include "file.hpp"
#include "square.hpp"
#include "strong_value.hpp"

class EnPassant : public StrongValue<EnPassant, uint8_t> {
   public:
    using StrongValue::StrongValue;

    [[nodiscard]] constexpr bool hasValue() const { return (m_value & (1 << 3)) != 0; }
    static constexpr uint8_t     bitlength() { return 4; }
    static constexpr ValueType   bitmask() { return static_cast<ValueType>((ValueType(1) << bitlength()) - 1); }

    constexpr void set(File file) noexcept { m_value = ((1 << 3) | file.value()); }
    constexpr void clear() noexcept { m_value = 0; }

    [[nodiscard]] constexpr File file() const noexcept { return File(m_value & File::bitmask()); }
};
