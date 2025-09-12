#pragma once

#include <type_traits>
template <typename T>
struct Dimensions {
    static_assert(std::is_arithmetic_v<T>, "Type should be arithmetic");

    T width;
    T height;

    constexpr Dimensions() = default;
    constexpr Dimensions(T width, T height) : width(width), height(height) {};
};