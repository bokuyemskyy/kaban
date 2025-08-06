#pragma once

#include <cstdint>
#include <iterator>
template <typename Derived>
struct Iterable {
    [[nodiscard]] static constexpr Derived first() { return Derived::FIRST; }
    [[nodiscard]] static constexpr Derived last() { return Derived::LAST; }

    class Iterator {
       private:
        uint8_t m_current;

       public:
        using iterator_category = std::forward_iterator_tag;
        using value_type        = Derived;
        using difference_type   = std::ptrdiff_t;
        using pointer           = const Derived*;
        using reference         = const Derived&;

        constexpr Iterator(uint8_t current) : m_current(current) {}

        constexpr Derived operator*() const { return m_current; }

        constexpr Iterator& operator++() {
            ++m_current;
            return *this;
        }

        constexpr Iterator operator++(int) {
            Iterator tmp = *this;
            ++m_current;
            return tmp;
        }

        constexpr bool operator==(const Iterator& other) const { return m_current == other.m_current; }

        constexpr bool operator!=(const Iterator& other) const { return m_current != other.m_current; }
    };

    class Range {
       public:
        [[nodiscard]] constexpr Iterator begin() const { return Derived::FIRST; }
        [[nodiscard]] constexpr Iterator end() const { return Derived::LAST + 1; }
    };

    [[nodiscard]] static constexpr Range all() { return Range{}; }
};
