#pragma once

#include <cassert>
#include <cstdint>
#include <type_traits>

template <typename Derived, typename Type, uint8_t BitWidth = 0>
struct StrongValue {
    using ValueType = Type;

    constexpr StrongValue() noexcept : m_value{} {}
    explicit constexpr StrongValue(Type value) noexcept : m_value(value) {}

    [[nodiscard]] constexpr Type value() const noexcept { return m_value; }

    friend constexpr bool operator==(const Derived& lhs, const Derived& rhs) noexcept {
        return lhs.value() == rhs.value();
    }
    friend constexpr auto operator<=>(const Derived& lhs, const Derived& rhs) noexcept {
        return lhs.value() <=> rhs.value();
    }

    static constexpr uint8_t width() noexcept
        requires(BitWidth > 0)
    {
        return BitWidth;
    }
    static constexpr ValueType mask() noexcept
        requires(BitWidth > 0)
    {
        return (ValueType(1) << BitWidth) - 1;
    }

    ValueType m_value;
};
