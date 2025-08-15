#pragma once

#include <array>
#include <cctype>
#include <cstdint>

#include "piece.hpp"
#include "strong_enum.hpp"

class Rank : public StrongEnum<Rank, uint8_t> {
   public:
    using StrongEnum<Rank, uint8_t>::StrongEnum;

    static constexpr uint8_t distance(Rank from, Rank to) {
        return from.value() > to.value() ? from.value() - to.value() : to.value() - from.value();
    }

    static constexpr Rank fromChar(char c) {
        c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
        for (uint8_t i = 0; i < number(); ++i)
            if (chars[i] == c) return Rank(i);
        return Rank();
    }
    [[nodiscard]] constexpr char toChar() const { return isValid() ? chars[m_value] : '?'; }

    [[nodiscard]] constexpr bool isPawnStart(Color color) const {
        return (color == Color::WHITE && value() == Values::R2) || (color == Color::BLACK && value() == Values::R7);
    }
    [[nodiscard]] constexpr bool isPawnPromotion(Color color) const {
        return (color == Color::WHITE && value() == Values::R8) || (color == Color::BLACK && value() == Values::R1);
    }

   private:
    static constexpr std::array<char, 8> chars = {'1', '2', '3', '4', '5', '6', '7', '8'};

    // clang-format off
    enum Values : uint8_t {
        R1 = 0, R2, R3, R4, R5, R6, R7, R8,
        NONE = 0xFF
    };
    // clang-format on

    enum Metadata : uint8_t {
        MASK   = 0b111,
        SIZE   = 3,
        NUMBER = 8
    };

    friend struct Ranks;
    friend class StrongEnum<Rank, uint8_t>;
};

struct Ranks {
    static constexpr Rank R1{Rank::Values::R1};
    static constexpr Rank R2{Rank::Values::R2};
    static constexpr Rank R3{Rank::Values::R3};
    static constexpr Rank R4{Rank::Values::R4};
    static constexpr Rank R5{Rank::Values::R5};
    static constexpr Rank R6{Rank::Values::R6};
    static constexpr Rank R7{Rank::Values::R7};
    static constexpr Rank R8{Rank::Values::R8};

    static constexpr std::array<Rank, Rank::number()> all() { return {R1, R2, R3, R4, R5, R6, R7, R8}; }
};
