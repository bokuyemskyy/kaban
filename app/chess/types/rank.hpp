#pragma once

#include <array>
#include <cstdint>

#include "strong_value.hpp"

struct Rank : public StrongValue<Rank, uint8_t, 3> {
    using StrongValue::StrongValue;

    [[nodiscard]] constexpr bool hasValue() const { return m_value < 8; }

    static constexpr uint8_t distance(Rank from, Rank to) noexcept {
        return from.m_value > to.m_value ? from.m_value - to.m_value : to.m_value - from.m_value;
    }

    static constexpr Rank fromChar(char c) noexcept {
        if (c >= '1' && c <= '8') return Rank(static_cast<uint8_t>(c - '1'));
        return Rank();
    }

    [[nodiscard]] constexpr char toChar() const noexcept { return hasValue() ? static_cast<char>('1' + m_value) : '?'; }

    constexpr Rank operator+(int offset) const noexcept {
        return Rank(static_cast<uint8_t>(static_cast<int>(m_value) + offset));
    }
    constexpr Rank operator-(int offset) const noexcept {
        return Rank(static_cast<uint8_t>(static_cast<int>(m_value) - offset));
    }
    
    constexpr Rank& operator++() noexcept {
        ++m_value;
        return *this;
    }

    constexpr Rank operator++(int) noexcept {
        Rank tmp = *this;
        ++m_value;
        return tmp;
    }

    constexpr Rank& operator--() noexcept {
        --m_value;
        return *this;
    }

    constexpr Rank operator--(int) noexcept {
        Rank tmp = *this;
        --m_value;
        return tmp;
    }

    constexpr Rank& operator+=(int offset) noexcept {
        m_value = static_cast<uint8_t>(static_cast<int>(m_value) + offset);
        return *this;
    }

    constexpr Rank& operator-=(int offset) noexcept {
        m_value = static_cast<uint8_t>(static_cast<int>(m_value) - offset);
        return *this;
    }
    
    constexpr int operator-(Rank other) const noexcept {
        return static_cast<int>(m_value) - static_cast<int>(other.m_value);
    }

    constexpr Rank operator+(Rank other) const noexcept {
        return Rank(m_value + other.m_value);
    }

};

namespace Ranks {
inline constexpr Rank R1{0};
inline constexpr Rank R2{1};
inline constexpr Rank R3{2};
inline constexpr Rank R4{3};
inline constexpr Rank R5{4};
inline constexpr Rank R6{5};
inline constexpr Rank R7{6};
inline constexpr Rank R8{7};

constexpr uint8_t count() noexcept { return 8; }

constexpr std::array<Rank, count()> all() { return {R1, R2, R3, R4, R5, R6, R7, R8}; }
};  // namespace Ranks
