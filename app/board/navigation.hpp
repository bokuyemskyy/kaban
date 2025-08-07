#pragma once

#include <sys/types.h>

#include <array>
#include <cassert>
#include <cctype>
#include <cstdint>
#include <iostream>
#include <string>

#include "direction.hpp"
#include "piece.hpp"
#include "rect.hpp"

template <typename Derived>
struct Coordinate {
   public:
    constexpr void                  set(uint8_t value) { m_value = value; }
    [[nodiscard]] constexpr uint8_t value() const { return m_value; }

    [[nodiscard]] constexpr bool ok() const { return m_value <= Derived::LAST; }

    [[nodiscard]] static constexpr Derived none() { return Derived::NONE; }

    [[nodiscard]] static constexpr uint8_t number() { return Derived::NB; }
    [[nodiscard]] static constexpr uint8_t mask() { return Derived::MASK; }
    [[nodiscard]] static constexpr uint8_t size() { return Derived::SIZE; }

    constexpr operator uint8_t() { return m_value; }

    friend std::ostream& operator<<(std::ostream& os, const Derived& obj) {
        obj.print(os);
        return os;
    }

    friend Derived;

   private:
    uint8_t m_value;

    constexpr Coordinate(uint8_t value = none()) : m_value(value) {}
};

class File {
   public:
    constexpr File() = default;
    explicit constexpr File(uint8_t value) : m_value(value) {}

    [[nodiscard]] constexpr uint8_t value() const { return m_value; }

    static constexpr uint8_t distance(File from, File to) {
        return from.value() > to.value() ? from.value() - to.value() : to.value() - from.value();
    }

    static constexpr File fromChar(char c) {
        c = static_cast<char>(tolower(c));
        for (uint8_t i = 0; i < number(); ++i)
            if (chars[i] == c) return File(i);
        assert(false);
    }
    [[nodiscard]] char toChar() const {
        assert(isOk());
        return chars[m_value];
    }

    constexpr bool operator==(const File& other) const { return m_value == other.m_value; }
    constexpr auto operator<=>(const File& other) const = default;

    [[nodiscard]] static constexpr uint8_t number() { return Metadata::NUMBER; }
    [[nodiscard]] static constexpr uint8_t mask() { return Metadata::MASK; }
    [[nodiscard]] static constexpr uint8_t size() { return Metadata::SIZE; }

   private:
    uint8_t m_value;

    static constexpr std::array<char, 8> chars = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};

    // clang-format off
    enum Value : uint8_t { 
        FA = 0, FB, FC, FD, FE, FF, FG, FH,
    };
    // clang-format on
    enum Metadata : uint8_t {
        MASK   = 0b111,
        SIZE   = 3,
        NUMBER = 8
    };

    friend struct Files;

    constexpr static bool        isOk(uint8_t value) { return value < number(); }
    [[nodiscard]] constexpr bool isOk() const { return isOk(value()); }
};

struct Files {
    static constexpr File FA{File::Value::FA};
    static constexpr File FB{File::Value::FB};
    static constexpr File FC{File::Value::FC};
    static constexpr File FD{File::Value::FD};
    static constexpr File FE{File::Value::FE};
    static constexpr File FF{File::Value::FF};
    static constexpr File FG{File::Value::FG};
    static constexpr File FH{File::Value::FH};

    static constexpr std::array<File, File::number()> all() { return {FA, FB, FC, FD, FE, FF, FG, FH}; }
};
class Rank {
   public:
    constexpr Rank() = default;
    explicit constexpr Rank(uint8_t value) : m_value(value) {}

    [[nodiscard]] constexpr uint8_t value() const { return m_value; }

    static constexpr uint8_t distance(Rank from, Rank to) {
        return from.value() > to.value() ? from.value() - to.value() : to.value() - from.value();
    }

    static constexpr Rank fromChar(char c) {
        c = static_cast<char>(tolower(c));
        for (uint8_t i = 0; i < number(); ++i)
            if (chars[i] == c) return Rank(i);
        assert(false);
    }
    [[nodiscard]] char toChar() const {
        assert(isOk());
        return chars[m_value];
    }

    [[nodiscard]] constexpr bool isPawnStart(Color color) const {
        return (color == Color::WHITE && value() == Value::R2) || (color == Color::BLACK && value() == Value::R7);
    }
    [[nodiscard]] constexpr bool isPawnPromotion(Color color) const {
        return (color == Color::WHITE && value() == Value::R8) || (color == Color::BLACK && value() == Value::R1);
    }

    constexpr bool operator==(const Rank& other) const { return m_value == other.m_value; }
    constexpr auto operator<=>(const Rank& other) const = default;

    [[nodiscard]] static constexpr uint8_t number() { return Metadata::NUMBER; }
    [[nodiscard]] static constexpr uint8_t mask() { return Metadata::MASK; }
    [[nodiscard]] static constexpr uint8_t size() { return Metadata::SIZE; }

   private:
    uint8_t m_value;

    static constexpr std::array<char, 8> chars = {'1', '2', '3', '4', '5', '6', '7', '8'};

    // clang-format off
    enum Value : uint8_t { 
        R1 = 0, R2, R3, R4, R5, R6, R7, R8
    };
    // clang-format on
    enum Metadata : uint8_t {
        MASK   = 0b111,
        SIZE   = 3,
        NUMBER = 8
    };

    friend struct Ranks;

    constexpr static bool        isOk(uint8_t value) { return value < number(); }
    [[nodiscard]] constexpr bool isOk() const { return isOk(value()); }
};

struct Ranks {
    static constexpr Rank R1{Rank::Value::R1};
    static constexpr Rank R2{Rank::Value::R2};
    static constexpr Rank R3{Rank::Value::R3};
    static constexpr Rank R4{Rank::Value::R4};
    static constexpr Rank R5{Rank::Value::R5};
    static constexpr Rank R6{Rank::Value::R6};
    static constexpr Rank R7{Rank::Value::R7};
    static constexpr Rank R8{Rank::Value::R8};

    static constexpr std::array<Rank, Rank::number()> all() { return {R1, R2, R3, R4, R5, R6, R7, R8}; }
};

class Square {
   public:
    constexpr Square() = default;
    explicit constexpr Square(uint8_t value) : m_value(value) {}
    explicit constexpr Square(File file, Rank rank) : m_value((rank.value() << File::size()) | file.value()) {}

    [[nodiscard]] constexpr uint8_t value() const { return m_value; }

    static constexpr Square fromString(const std::string& str) {
        assert(str.size() == 2);
        return Square(File::fromChar(str[0]), Rank::fromChar(str[1]));
    }
    [[nodiscard]] constexpr std::string toString() const {
        return std::string(1, file().toChar()) + std::string(1, rank().toChar());
    }

    [[nodiscard]] constexpr File file() const { return File(value() & File::mask()); }
    [[nodiscard]] constexpr Rank rank() const { return Rank(m_value >> File::size()); }

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
            for (uint8_t i = Metadata::FIRST; i <= Metadata::LAST; i++) {
                for (uint8_t j = Metadata::FIRST; j <= Metadata::LAST; j++) {
                    t[i][j] = std::max(File::distance(Square(i).file(), Square(j).file()),
                                       Rank::distance(Square(i).rank(), Square(j).rank()));
                }
            }
            return t;
        }();
        return table[from.value()][to.value()];
    }

    constexpr bool operator==(const Square& other) const { return value() == other.value(); }
    constexpr auto operator<=>(const Square& other) const = default;

    constexpr Square operator+(const Direction& direction) const {
        int16_t new_value = static_cast<int16_t>(value()) + direction.value();
        return Square(static_cast<uint8_t>(new_value));
    }

    [[nodiscard]] static constexpr uint8_t number() { return Metadata::NUMBER; }
    [[nodiscard]] static constexpr uint8_t mask() { return Metadata::MASK; }
    [[nodiscard]] static constexpr uint8_t size() { return Metadata::SIZE; }

    [[nodiscard]] constexpr bool isOk() const { return isOk(value()); }

   private:
    uint8_t m_value;

    // clang-format off
    enum Value : uint8_t { 
        A1, B1, C1, D1, E1, F1, G1, H1,
        A2, B2, C2, D2, E2, F2, G2, H2,
        A3, B3, C3, D3, E3, F3, G3, H3,
        A4, B4, C4, D4, E4, F4, G4, H4,
        A5, B5, C5, D5, E5, F5, G5, H5,
        A6, B6, C6, D6, E6, F6, G6, H6,
        A7, B7, C7, D7, E7, F7, G7, H7,
        A8, B8, C8, D8, E8, F8, G8, H8
    };
    // clang-format on
    enum Metadata : uint8_t {
        FIRST = A1,
        LAST  = H8,

        MASK   = 0b111111,
        SIZE   = 6,
        NUMBER = 64
    };

    friend struct Squares;

    constexpr static bool isOk(uint8_t value) { return value < number(); }
};

struct Squares {
    // clang-format off

    #define SQ(name) \
        static constexpr Square name { Square::Value::name }
    
        SQ(A1); SQ(A2); SQ(A3); SQ(A4); SQ(A5); SQ(A6); SQ(A7); SQ(A8); 
        SQ(B1); SQ(B2); SQ(B3); SQ(B4); SQ(B5); SQ(B6); SQ(B7); SQ(B8); 
        SQ(C1); SQ(C2); SQ(C3); SQ(C4); SQ(C5); SQ(C6); SQ(C7); SQ(C8); 
        SQ(D1); SQ(D2); SQ(D3); SQ(D4); SQ(D5); SQ(D6); SQ(D7); SQ(D8); 
        SQ(E1); SQ(E2); SQ(E3); SQ(E4); SQ(E5); SQ(E6); SQ(E7); SQ(E8); 
        SQ(F1); SQ(F2); SQ(F3); SQ(F4); SQ(F5); SQ(F6); SQ(F7); SQ(F8); 
        SQ(G1); SQ(G2); SQ(G3); SQ(G4); SQ(G5); SQ(G6); SQ(G7); SQ(G8); 
        SQ(H1); SQ(H2); SQ(H3); SQ(H4); SQ(H5); SQ(H6); SQ(H7); SQ(H8);
    
    #undef SQ

    // clang-format on

    static constexpr std::array<Square, Square::number()> all() { return {A1, A2, A3}; }
};

/*
constexpr std::array<Direction, 1> whitePawnPushDirections = {Direction::N};

constexpr std::array<Direction, 2> whitePawnAttackDirections = {Direction::NE, Direction::NW};

constexpr std::array<Direction, 8> queenDirections = {Direction::NE, Direction::NW, Direction::SE, Direction::SW,
                                                      Direction::N,  Direction::S,  Direction::E,  Direction::W};
*/