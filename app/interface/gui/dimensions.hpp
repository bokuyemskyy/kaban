#pragma once

#include <type_traits>
template <typename T>
struct Dimensions {
    static_assert(std::is_arithmetic_v<T>, "Type should be arithmetic");

    T width  = 0;
    T height = 0;

    constexpr Dimensions() = default;
    constexpr Dimensions(T _width, T _height) : width(_width), height(_height) {};
};