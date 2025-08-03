#pragma once

#include <type_traits>
template <typename T>
struct Dimension {
    static_assert(std::is_arithmetic_v<T>, "Type should be arithmetic");

    T width;
    T height;

    constexpr Dimension() = default;
    constexpr Dimension(T width, T height) : width(width), height(height) {};
};