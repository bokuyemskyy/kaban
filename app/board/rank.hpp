#pragma once

#include <array>
#include <cstdint>

#include "color.hpp"
#include "strong.hpp"

class Rank : public Strong::Value<Rank, uint8_t>,
             public Strong::Enumerator<Rank, uint8_t, 8>,
             public Strong::Field<Rank, uint8_t, 3> {
   public:
    using Value::Value;

    static constexpr uint8_t distance(Rank from, Rank to) noexcept {
        return from.value() > to.value() ? from.value() - to.value() : to.value() - from.value();
    }

    static constexpr Rank fromChar(char c) noexcept {
        if (c >= '1' && c <= '8') return Rank(static_cast<uint8_t>(c - '1'));
        return Rank();
    }

    [[nodiscard]] constexpr char toChar() const noexcept { return hasValue() ? static_cast<char>('1' + value()) : '?'; }

    [[nodiscard]] constexpr bool isPawnStart(const Color color) const {
        return (color == Colors::WHITE && value() == Values::R2) || (color == Colors::BLACK && value() == Values::R7);
    }
    [[nodiscard]] constexpr bool isPawnPromotion(const Color color) const {
        return (color == Colors::WHITE && value() == Values::R8) || (color == Colors::BLACK && value() == Values::R1);
    }

    constexpr Rank operator+(int offset) const noexcept {
        return Rank(static_cast<uint8_t>(static_cast<int>(value()) + offset));
    }
    constexpr Rank  operator-(int offset) const noexcept { return *this + (-offset); }
    constexpr Rank& operator+=(int offset) noexcept { return *this = *this + offset; }
    constexpr Rank& operator-=(int offset) noexcept { return *this = *this - offset; }

   private:
    // clang-format off
    enum Values : uint8_t {
        R1 = 0, R2, R3, R4, R5, R6, R7, R8,
        NONE = 0xFF
    };
    // clang-format on

    friend struct Ranks;
};

struct Ranks {
    // clang-format off
    #define R(name) \
        static constexpr Rank name { Rank::Values::name }

        R(R1); R(R2); R(R3); R(R4); R(R5); R(R6); R(R7); R(R8);
    #undef R

    static constexpr std::array<Rank, Rank::number()> all() { return {R1, R2, R3, R4, R5, R6, R7, R8}; }
    // clang-format on
};
