#pragma once

#include <algorithm>
#include <type_traits>

template <typename T>
struct Rect {
    static_assert(std::is_arithmetic_v<T>, "Type should be arithmetic");

    T x;
    T y;
    T width;
    T height;

    constexpr Rect() = default;
    constexpr Rect(T _x, T _y, T _width, T _height) : x(_x), y(_y), width(_width), height(_height) {}

    constexpr T top() const { return y; }
    constexpr T left() const { return x; }
    constexpr T right() const { return x + width; }
    constexpr T bottom() const { return y + height; }
};

template <typename T>
struct NormalizedRect {
    static_assert(std::is_floating_point_v<T>, "Type should be a floating point number");

    T x;
    T y;
    T width;
    T height;

    constexpr NormalizedRect() = default;
    constexpr NormalizedRect(T _x, T _y, T _width, T _height)
        : x(std::clamp(_x, static_cast<T>(0), static_cast<T>(1))),
          y(std::clamp(_y, static_cast<T>(0), static_cast<T>(1))),
          width(std::clamp(_width, static_cast<T>(0), static_cast<T>(1))),
          height(std::clamp(_height, static_cast<T>(0), static_cast<T>(1))) {}

    constexpr T top() const { return y; }
    constexpr T left() const { return x; }
    constexpr T right() const { return x + width; }
    constexpr T bottom() const { return y + height; }

    template <typename P>
    constexpr Rect<P> absolute(P absolute_width, P absolute_height) const {
        return {x * absolute_width, y * absolute_height, width * absolute_width, height * absolute_height};
    }
};