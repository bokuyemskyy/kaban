#pragma once

#include <algorithm>
#include <array>
#include <cstdint>
#include <string>

#include "direction.hpp"
#include "file.hpp"
#include "rank.hpp"
#include "strong_value.hpp"

struct Square : public StrongValue<Square, uint8_t, 6> {
    using StrongValue::StrongValue;

    explicit constexpr Square(uint8_t v) noexcept : StrongValue((v < 64) ? v : 64) {}
    explicit constexpr Square(File file, Rank rank) noexcept
        : StrongValue((rank.value() << File::width()) | file.value()) {}

    [[nodiscard]] constexpr Square operator+(const Direction& other) const { return Square(m_value + other.value()); }
    [[nodiscard]] constexpr Square operator-(const Direction& other) const { return Square(m_value - other.value()); }

    [[nodiscard]] constexpr File file() const noexcept { return File(value() & File::mask()); }
    [[nodiscard]] constexpr Rank rank() const noexcept { return Rank(value() >> File::width()); }

    static constexpr Square fromString(const std::string& str) {
        return Square(File::fromChar(str[0]), Rank::fromChar(str[1]));
    }

    [[nodiscard]] constexpr std::string toString() const { return {file().toChar(), rank().toChar()}; }

    [[nodiscard]] constexpr bool light() const noexcept { return ((file().value() + rank().value()) % 2) == 1; }

    static constexpr uint8_t distance(Square from, Square to) noexcept {
        static constexpr auto table = []() constexpr {
            std::array<std::array<uint8_t, 64>, 64> t{};
            for (uint8_t i = 0; i <= 63; i++) {
                for (uint8_t j = 0; j <= 63; j++) {
                    t[i][j] = std::max(File::distance(Square(i).file(), Square(j).file()),
                                       Rank::distance(Square(i).rank(), Square(j).rank()));
                }
            }
            return t;
        }();
        return table[from.value()][to.value()];
    }

    [[nodiscard]] constexpr Square move(const Direction& direction) const noexcept {
        File new_file = file() + direction.horizontal();
        Rank new_rank = rank() + direction.vertical();

        if (!new_file.hasValue() || !new_rank.hasValue()) {
            return Square(64);
        }

        return Square(new_file, new_rank);
    }
};

namespace Squares {
inline constexpr Square A1{0};
inline constexpr Square B1{1};
inline constexpr Square C1{2};
inline constexpr Square D1{3};
inline constexpr Square E1{4};
inline constexpr Square F1{5};
inline constexpr Square G1{6};
inline constexpr Square H1{7};

inline constexpr Square A2{8};
inline constexpr Square B2{9};
inline constexpr Square C2{10};
inline constexpr Square D2{11};
inline constexpr Square E2{12};
inline constexpr Square F2{13};
inline constexpr Square G2{14};
inline constexpr Square H2{15};

inline constexpr Square A3{16};
inline constexpr Square B3{17};
inline constexpr Square C3{18};
inline constexpr Square D3{19};
inline constexpr Square E3{20};
inline constexpr Square F3{21};
inline constexpr Square G3{22};
inline constexpr Square H3{23};

inline constexpr Square A4{24};
inline constexpr Square B4{25};
inline constexpr Square C4{26};
inline constexpr Square D4{27};
inline constexpr Square E4{28};
inline constexpr Square F4{29};
inline constexpr Square G4{30};
inline constexpr Square H4{31};

inline constexpr Square A5{32};
inline constexpr Square B5{33};
inline constexpr Square C5{34};
inline constexpr Square D5{35};
inline constexpr Square E5{36};
inline constexpr Square F5{37};
inline constexpr Square G5{38};
inline constexpr Square H5{39};

inline constexpr Square A6{40};
inline constexpr Square B6{41};
inline constexpr Square C6{42};
inline constexpr Square D6{43};
inline constexpr Square E6{44};
inline constexpr Square F6{45};
inline constexpr Square G6{46};
inline constexpr Square H6{47};

inline constexpr Square A7{48};
inline constexpr Square B7{49};
inline constexpr Square C7{50};
inline constexpr Square D7{51};
inline constexpr Square E7{52};
inline constexpr Square F7{53};
inline constexpr Square G7{54};
inline constexpr Square H7{55};

inline constexpr Square A8{56};
inline constexpr Square B8{57};
inline constexpr Square C8{58};
inline constexpr Square D8{59};
inline constexpr Square E8{60};
inline constexpr Square F8{61};
inline constexpr Square G8{62};
inline constexpr Square H8{63};

inline constexpr Square NONE{64};

constexpr uint8_t count() noexcept { return 64; }

constexpr std::array<Square, count()> all() {
    std::array<Square, count()> arr{};
    for (uint8_t i = 0; i < count(); ++i) {
        arr[i] = Square(i);
    }
    return arr;
}
}  // namespace Squares
