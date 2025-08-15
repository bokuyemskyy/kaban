#pragma once

#include <cstdint>
#include <string>

#include "direction.hpp"
#include "file.hpp"
#include "rank.hpp"
#include "rect.hpp"
#include "strong_enum.hpp"

class Square : public StrongEnum<Square, uint8_t> {
   public:
    using StrongEnum<Square, uint8_t>::StrongEnum;

    explicit constexpr Square(File file, Rank rank)
        : StrongEnum(static_cast<uint8_t>((rank.value() << File::size()) | file.value())) {}

    [[nodiscard]] constexpr File file() const { return File(value() & File::mask()); }
    [[nodiscard]] constexpr Rank rank() const { return Rank(value() >> File::size()); }

    static constexpr Square fromString(const std::string& str) {
        return Square(File::fromChar(str[0]), Rank::fromChar(str[1]));
    }
    [[nodiscard]] constexpr std::string toString() const {
        return std::string(1, file().toChar()) + std::string(1, rank().toChar());
    }

    [[nodiscard]] constexpr bool light() const { return (file().value() + rank().value()) % 2 == 0; }

    [[nodiscard]] constexpr NormalizedRect<float> normalizedRect() const {
        return {static_cast<float>(file().value()) / static_cast<float>(File::number()),
                1 - (1 / static_cast<float>(Rank::number())) -
                    (static_cast<float>(rank().value()) / static_cast<float>(Rank::number())),
                1 / static_cast<float>(File::number()), 1 / static_cast<float>(Rank::number())};
    }

    static constexpr uint8_t distance(Square from, Square to) {
        static constexpr auto table = []() constexpr {
            std::array<std::array<uint8_t, NUMBER>, NUMBER> t{};
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

    constexpr Square operator+(const Direction& direction) const {
        int16_t new_value = static_cast<int16_t>(value()) + direction.value();
        return Square(static_cast<uint8_t>(new_value));
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
        NONE = 0xFF
    };
    // clang-format on
    enum Metadata : uint8_t {
        MASK   = 0b111111,
        SIZE   = 6,
        NUMBER = 64
    };

    friend struct Squares;
    friend class StrongEnum<Square, uint8_t>;
};

struct Squares {
    // clang-format off

    #define SQ(name) \
        static constexpr Square name { Square::Values::name }
    
        SQ(A1); SQ(A2); SQ(A3); SQ(A4); SQ(A5); SQ(A6); SQ(A7); SQ(A8); 
        SQ(B1); SQ(B2); SQ(B3); SQ(B4); SQ(B5); SQ(B6); SQ(B7); SQ(B8); 
        SQ(C1); SQ(C2); SQ(C3); SQ(C4); SQ(C5); SQ(C6); SQ(C7); SQ(C8); 
        SQ(D1); SQ(D2); SQ(D3); SQ(D4); SQ(D5); SQ(D6); SQ(D7); SQ(D8); 
        SQ(E1); SQ(E2); SQ(E3); SQ(E4); SQ(E5); SQ(E6); SQ(E7); SQ(E8); 
        SQ(F1); SQ(F2); SQ(F3); SQ(F4); SQ(F5); SQ(F6); SQ(F7); SQ(F8); 
        SQ(G1); SQ(G2); SQ(G3); SQ(G4); SQ(G5); SQ(G6); SQ(G7); SQ(G8); 
        SQ(H1); SQ(H2); SQ(H3); SQ(H4); SQ(H5); SQ(H6); SQ(H7); SQ(H8);
    
    #undef SQ

    
    static constexpr std::array<Square, Square::number()> all() {
        return {A1, A2, A3, A4, A5, A6, A7, A8,
                B1, B2, B3, B4, B5, B6, B7, B8,
                C1, C2, C3, C4, C5, C6, C7, C8,
                D1, D2, D3, D4, D5, D6, D7, D8,
                E1, E2, E3, E4, E5, E6, E7, E8,
                F1, F2, F3, F4, F5, F6, F7, F8,
                G1, G2, G3, G4, G5, G6, G7, G8,
                H1, H2, H3, H4, H5, H6, H7, H8};
            }

    // clang-format on
};
