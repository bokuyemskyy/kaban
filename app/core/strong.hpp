#pragma once
#include <bit>

namespace Strong {
template <typename Derived, typename T>
class Value {
   public:
    using ValueType = T;

    constexpr Value() noexcept = default;
    explicit constexpr Value(T value) noexcept : m_value(value) {}

    constexpr void            set(T value) { m_value = value; };
    [[nodiscard]] constexpr T value() const { return m_value; }

    friend constexpr bool operator==(const Derived& lhs, const Derived& rhs) noexcept {
        return lhs.value() == rhs.value();
    }
    friend constexpr auto operator<=>(const Derived& lhs, const Derived& rhs) noexcept {
        return lhs.value() <=> rhs.value();
    }

   protected:
    T m_value{};
};

template <typename Derived, typename T, auto Number>
class Enumerator {
   public:
    static constexpr T           number() noexcept { return static_cast<T>(Number); }
    [[nodiscard]] constexpr bool hasValue() const { return static_cast<const Derived&>(*this).value() < number(); }
};

template <typename Derived, typename T, unsigned Bits>
class Field {
   public:
    static_assert(Bits > 0 && Bits <= sizeof(T) * 8, "Invalid number of bits");

    static constexpr T        mask() { return (T(1) << Bits) - 1; }
    static constexpr unsigned size() { return Bits; }
};
}  // namespace Strong