#pragma once

#include <type_traits>
template <typename T>
struct Dimensions {
    static_assert(std::is_arithmetic_v<T>, "Type should be arithmetic");

    T width;
    T height;

    constexpr Dimensions() = default;
    constexpr Dimensions(T _width, T _height) : width(_width), height(_height) {};
};