#pragma once

#include <cstdint>

#include "strong.hpp"

class Halfmove : public Strong::Value<Halfmove, uint8_t>, public Strong::Field<Halfmove, uint8_t, 7> {
   public:
    using Value::Value;

    constexpr Halfmove& operator++() noexcept {
        ++m_value;
        return *this;
    }

    constexpr Halfmove& reset() noexcept {
        m_value = 0;
        return *this;
    }

    [[nodiscard]] constexpr bool isDraw() const noexcept { return m_value >= 100; }
};
