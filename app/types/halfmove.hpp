#pragma once

#include <algorithm>
#include <cstdint>

#include "strong_value.hpp"

struct Halfmove : public StrongValue<Halfmove, uint8_t, 7> {
    using StrongValue::StrongValue;

    constexpr Halfmove& operator++() noexcept {
        if (m_value < 100) {
            ++m_value;
        }
        return *this;
    }

    constexpr Halfmove& reset() noexcept {
        m_value = 0;
        return *this;
    }

    constexpr void set(uint8_t new_value) {
        new_value = std::min<uint8_t>(new_value, 100);
        m_value   = new_value;
    }

    [[nodiscard]] constexpr bool isDraw() const noexcept { return m_value >= 100; }
};
