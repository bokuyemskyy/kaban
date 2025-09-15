
#pragma once

#include <algorithm>
#include <array>
#include <cstdint>
#include <string>

#include "direction.hpp"
#include "file.hpp"
#include "rank.hpp"
#include "rect.hpp"
#include "strong_value.hpp"

class Square : public StrongValue<Square, uint8_t> {
   public:
    using StrongValue::StrongValue;

    static constexpr ValueType   count() noexcept { return static_cast<ValueType>(64); }
    [[nodiscard]] constexpr bool hasValue() const { return value() < count(); }
    static constexpr uint8_t     bitlength() { return 6; }
    static constexpr ValueType   bitmask() { return static_cast<ValueType>((ValueType(1) << bitlength()) - 1); }

    constexpr Square(File file, Rank rank) noexcept
        : StrongValue(static_cast<uint8_t>((rank.value() << File::bitlength()) | file.value())) {}

    [[nodiscard]] constexpr File file() const noexcept { return File(value() & File::bitmask()); }

    [[nodiscard]] constexpr Rank rank() const noexcept { return Rank(value() >> File::bitlength()); }

    static constexpr Square fromString(const std::string& str) {
        return Square(File::fromChar(str[0]), Rank::fromChar(str[1]));
    }

    [[nodiscard]] constexpr std::string toString() const { return {file().toChar(), rank().toChar()}; }

    [[nodiscard]] constexpr bool light() const noexcept { return ((file().value() + rank().value()) % 2) == 1; }

    [[nodiscard]] constexpr NormalizedRect<float> normalized_rect() const noexcept {
        return {static_cast<float>(file().value()) / File::count(),
                1 - (1.0f / Rank::count()) - (static_cast<float>(rank().value()) / Rank::count()), 1.0f / File::count(),
                1.0f / Rank::count()};
    }

    static constexpr uint8_t distance(Square from, Square to) noexcept {
        static constexpr auto table = []() constexpr {
            std::array<std::array<uint8_t, count()>, count()> t{};
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

    [[nodiscard]] constexpr Square shifted(const Direction& direction) const noexcept {
        File newFile = file() + direction.horizontal();
        Rank newRank = rank() + direction.vertical();

        if (!newFile.hasValue() || !newRank.hasValue()) {
            return Square(Values::NONE);
        }

        return Square(newFile, newRank);
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
    static constexpr Square A1{Square::Values::A1};
    static constexpr Square B1{Square::Values::B1};
    static constexpr Square C1{Square::Values::C1};
    static constexpr Square D1{Square::Values::D1};
    static constexpr Square E1{Square::Values::E1};
    static constexpr Square F1{Square::Values::F1};
    static constexpr Square G1{Square::Values::G1};
    static constexpr Square H1{Square::Values::H1};
    static constexpr Square A2{Square::Values::A2};
    static constexpr Square B2{Square::Values::B2};
    static constexpr Square C2{Square::Values::C2};
    static constexpr Square D2{Square::Values::D2};
    static constexpr Square E2{Square::Values::E2};
    static constexpr Square F2{Square::Values::F2};
    static constexpr Square G2{Square::Values::G2};
    static constexpr Square H2{Square::Values::H2};
    static constexpr Square A3{Square::Values::A3};
    static constexpr Square B3{Square::Values::B3};
    static constexpr Square C3{Square::Values::C3};
    static constexpr Square D3{Square::Values::D3};
    static constexpr Square E3{Square::Values::E3};
    static constexpr Square F3{Square::Values::F3};
    static constexpr Square G3{Square::Values::G3};
    static constexpr Square H3{Square::Values::H3};
    static constexpr Square A4{Square::Values::A4};
    static constexpr Square B4{Square::Values::B4};
    static constexpr Square C4{Square::Values::C4};
    static constexpr Square D4{Square::Values::D4};
    static constexpr Square E4{Square::Values::E4};
    static constexpr Square F4{Square::Values::F4};
    static constexpr Square G4{Square::Values::G4};
    static constexpr Square H4{Square::Values::H4};
    static constexpr Square A5{Square::Values::A5};
    static constexpr Square B5{Square::Values::B5};
    static constexpr Square C5{Square::Values::C5};
    static constexpr Square D5{Square::Values::D5};
    static constexpr Square E5{Square::Values::E5};
    static constexpr Square F5{Square::Values::F5};
    static constexpr Square G5{Square::Values::G5};
    static constexpr Square H5{Square::Values::H5};
    static constexpr Square A6{Square::Values::A6};
    static constexpr Square B6{Square::Values::B6};
    static constexpr Square C6{Square::Values::C6};
    static constexpr Square D6{Square::Values::D6};
    static constexpr Square E6{Square::Values::E6};
    static constexpr Square F6{Square::Values::F6};
    static constexpr Square G6{Square::Values::G6};
    static constexpr Square H6{Square::Values::H6};
    static constexpr Square A7{Square::Values::A7};
    static constexpr Square B7{Square::Values::B7};
    static constexpr Square C7{Square::Values::C7};
    static constexpr Square D7{Square::Values::D7};
    static constexpr Square E7{Square::Values::E7};
    static constexpr Square F7{Square::Values::F7};
    static constexpr Square G7{Square::Values::G7};
    static constexpr Square H7{Square::Values::H7};
    static constexpr Square A8{Square::Values::A8};
    static constexpr Square B8{Square::Values::B8};
    static constexpr Square C8{Square::Values::C8};
    static constexpr Square D8{Square::Values::D8};
    static constexpr Square E8{Square::Values::E8};
    static constexpr Square F8{Square::Values::F8};
    static constexpr Square G8{Square::Values::G8};
    static constexpr Square H8{Square::Values::H8};
    static constexpr Square NONE{Square::Values::NONE};

    static constexpr std::array<Square, Square::count()> all() {
        std::array<Square, Square::count()> arr{};
        for (uint8_t i = 0; i < Square::count(); ++i) {
            arr[i] = Square{i};
        }
        return arr;
    }
};
