#pragma once

#include <cstdint>

#include "strong_value.hpp"

class Halfmove : public StrongValue<Halfmove, uint8_t> {
   public:
    using StrongValue::StrongValue;

    static constexpr uint8_t   bitlen() { return 7; }
    static constexpr ValueType bitmask() { return static_cast<ValueType>((ValueType(1) << bitlen()) - 1); }

    constexpr Halfmove& operator++() noexcept {
        ++m_value;
        return *this;
    }

    constexpr Halfmove& reset() noexcept {
        m_value = 0;
        return *this;
    }

    [[nodiscard]] constexpr bool is_draw() const noexcept { return m_value >= 100; }
};
