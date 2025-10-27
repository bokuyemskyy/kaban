#pragma once

#include <array>
#include <cstdint>

#include "strong_value.hpp"

struct File : public StrongValue<File, uint8_t, 3> {
    using StrongValue::StrongValue;

    [[nodiscard]] constexpr bool hasValue() const { return m_value < 8; }

    static constexpr uint8_t distance(File from, File to) noexcept {
        return from.m_value > to.m_value ? from.m_value - to.m_value : to.m_value - from.m_value;
    }

    static constexpr File fromChar(char c) noexcept {
        if (c >= 'a' && c <= 'h') return File(static_cast<uint8_t>(c - 'a'));
        return File();
    }

    [[nodiscard]] constexpr char toChar() const noexcept { return hasValue() ? static_cast<char>('a' + m_value) : '?'; }

    constexpr File operator+(int offset) const noexcept {
        return File(static_cast<uint8_t>(static_cast<int>(m_value) + offset));
    }
    constexpr File operator-(int offset) const noexcept {
        return File(static_cast<uint8_t>(static_cast<int>(m_value) - offset));
    }

    constexpr File& operator++() noexcept {
        ++m_value;
        return *this;
    }

    constexpr File operator++(int) noexcept {
        File tmp = *this;
        ++m_value;
        return tmp;
    }

    constexpr File& operator--() noexcept {
        --m_value;
        return *this;
    }

    constexpr File operator--(int) noexcept {
        File tmp = *this;
        --m_value;
        return tmp;
    }

    constexpr File& operator+=(int offset) noexcept {
        m_value = static_cast<uint8_t>(static_cast<int>(m_value) + offset);
        return *this;
    }

    constexpr File& operator-=(int offset) noexcept {
        m_value = static_cast<uint8_t>(static_cast<int>(m_value) - offset);
        return *this;
    }
    
    constexpr int operator-(File other) const noexcept {
        return static_cast<int>(m_value) - static_cast<int>(other.m_value);
    }

    constexpr File operator+(File other) const noexcept {
        return File(m_value + other.m_value);
    }
};

namespace Files {
inline constexpr File FA{0};
inline constexpr File FB{1};
inline constexpr File FC{2};
inline constexpr File FD{3};
inline constexpr File FE{4};
inline constexpr File FF{5};
inline constexpr File FG{6};
inline constexpr File FH{7};

constexpr uint8_t count() noexcept { return 8; }

constexpr std::array<File, count()> all() { return {FA, FB, FC, FD, FE, FF, FG, FH}; }
};  // namespace Files
