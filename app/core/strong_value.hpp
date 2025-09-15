#pragma once

template <typename Derived, typename T>
class StrongValue {
   public:
    using ValueType = T;

    constexpr StrongValue() noexcept = default;
    explicit constexpr StrongValue(T value) noexcept : m_value(value) {}

    [[nodiscard]] constexpr T value() const { return m_value; }

    friend constexpr bool operator==(const Derived& lhs, const Derived& rhs) noexcept {
        return lhs.value() == rhs.value();
    }
    friend constexpr auto operator<=>(const Derived& lhs, const Derived& rhs) noexcept {
        return lhs.value() <=> rhs.value();
    }

    T m_value{};
};
