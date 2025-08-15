#pragma once

#include <algorithm>
#include <array>
#include <cstdint>
#include <string>

#include "direction.hpp"
#include "file.hpp"
#include "rank.hpp"
#include "rect.hpp"
#include "strong.hpp"

class Square : public Strong::Value<Square, uint8_t>,
               public Strong::Enumerator<Square, uint8_t, 64>,
               public Strong::Field<Square, uint8_t, 6> {
   public:
    using Value::Value;

    constexpr Square(File file, Rank rank) noexcept
        : Value(static_cast<uint8_t>((rank.value() << File::size()) | file.value())) {}

    [[nodiscard]] constexpr File file() const noexcept { return File(value() & File::mask()); }

    [[nodiscard]] constexpr Rank rank() const noexcept { return Rank(value() >> File::size()); }

    static constexpr Square fromString(const std::string& str) {
        return Square(File::fromChar(str[0]), Rank::fromChar(str[1]));
    }

    [[nodiscard]] constexpr std::string toString() const { return {file().toChar(), rank().toChar()}; }

    [[nodiscard]] constexpr bool light() const noexcept { return ((file().value() + rank().value()) % 2) == 1; }

    [[nodiscard]] constexpr NormalizedRect<float> normalizedRect() const noexcept {
        return {static_cast<float>(file().value()) / File::number(),
                1 - (1.0f / Rank::number()) - (static_cast<float>(rank().value()) / Rank::number()),
                1.0f / File::number(), 1.0f / Rank::number()};
    }

    static constexpr uint8_t distance(Square from, Square to) noexcept {
        static constexpr auto table = []() constexpr {
            std::array<std::array<uint8_t, number()>, number()> t{};
            for (uint8_t i = Values::A1; i <= Values::H8; i++) {
                for (uint8_t j = Values::A1; j <= Values::H8; j++) {
                    t[i][j] = std::max(File::distance(Square(i).file(), Square(j).file()),
                                       Rank::distance(Square(i).rank(), Square(j).rank()));
                }
            }
            return t;
        }();
        return table[from.value()][to.value()];
    }

    [[nodiscard]] constexpr Square moved(const Direction& direction) const noexcept {
        File newFile = file() + direction.horizontal();
        Rank newRank = rank() + direction.vertical();

        if (!newFile.hasValue() || !newRank.hasValue()) {
            return Square(Values::NONE);
        }

        return Square(newFile, newRank);
    }

    constexpr Square& move(const Direction& direction) noexcept {
        *this = moved(direction);
        return *this;
    }

   private:
    // clang-format off
    enum Values : uint8_t { 
        A1, B1, C1, D1, E1, F1, G1, H1,
        A2, B2, C2, D2, E2, F2, G2, H2,
        A3, B3, C3, D3, E3, F3, G3, H3,
        A4, B4, C4, D4, E4, F4, G4, H4,
        A5, B5, C5, D5, E5, F5, G5, H5,
        A6, B6, C6, D6, E6, F6, G6, H6,
        A7, B7, C7, D7, E7, F7, G7, H7,
        A8, B8, C8, D8, E8, F8, G8, H8,
        NONE = 64
    };
    // clang-format on

    friend struct Squares;
};

struct Squares {
    // clang-format off

    #define SQ(name) \
        static constexpr Square name { Square::Values::name }
    
        SQ(A1); SQ(B1); SQ(C1); SQ(D1); SQ(E1); SQ(F1); SQ(G1); SQ(H1); 
        SQ(A2); SQ(B2); SQ(C2); SQ(D2); SQ(E2); SQ(F2); SQ(G2); SQ(H2); 
        SQ(A3); SQ(B3); SQ(C3); SQ(D3); SQ(E3); SQ(F3); SQ(G3); SQ(H3); 
        SQ(A4); SQ(B4); SQ(C4); SQ(D4); SQ(E4); SQ(F4); SQ(G4); SQ(H4); 
        SQ(A5); SQ(B5); SQ(C5); SQ(D5); SQ(E5); SQ(F5); SQ(G5); SQ(H5); 
        SQ(A6); SQ(B6); SQ(C6); SQ(D6); SQ(E6); SQ(F6); SQ(G6); SQ(H6); 
        SQ(A7); SQ(B7); SQ(C7); SQ(D7); SQ(E7); SQ(F7); SQ(G7); SQ(H7); 
        SQ(A8); SQ(B8); SQ(C8); SQ(D8); SQ(E8); SQ(F8); SQ(G8); SQ(H8);
    
    #undef SQ

    
    static constexpr std::array<Square, Square::number()> all() {
        return {A1, B1, C1, D1, E1, F1, G1, H1,
                A2, B2, C2, D2, E2, F2, G2, H2,
                A3, B3, C3, D3, E3, F3, G3, H3,
                A4, B4, C4, D4, E4, F4, G4, H4,
                A5, B5, C5, D5, E5, F5, G5, H5,
                A6, B6, C6, D6, E6, F6, G6, H6,
                A7, B7, C7, D7, E7, F7, G7, H7,
                A8, B8, C8, D8, E8, F8, G8, H8};
            }

    // clang-format on
};
