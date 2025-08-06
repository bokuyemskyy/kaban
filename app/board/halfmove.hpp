#include <concepts>
#include <cstdint>

#include "glfw_wrapper.hpp"
#include "navigation.hpp"

// example of a clear, type-safe wrapper (i have to implement the other ones the same way)

struct Halfmove {
    constexpr Halfmove() : m_value(0) {}
    constexpr explicit Halfmove(uint8_t value) : m_value(value) {}

    [[nodiscard]] constexpr uint8_t value() const { return m_value; }

    constexpr Halfmove& operator++() {
        ++m_value;
        return *this;
    }
    constexpr bool operator==(const Halfmove& other) const { return m_value == other.m_value; }
    constexpr bool operator<(const Halfmove& other) const { return m_value < other.m_value; }

   private:
    uint8_t m_value;
};