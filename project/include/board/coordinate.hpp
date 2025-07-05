#ifndef NAVIGATION_HPP
#define NAVIGATION_HPP

#include <array>
#include <cstdint>
#include <iostream>
#include <string_view>

#include "piece.hpp"

template <typename Derived>
struct Coordinate {
   protected:
    uint8_t m_val;

   private:
    constexpr Coordinate(uint8_t val) : m_val(val) {}

   public:
    constexpr operator uint8_t() const { return m_val; }

    [[nodiscard]] constexpr bool isValid() const { return m_val <= Derived::LAST; }

    constexpr Derived& operator++() {
        ++m_val;
        return static_cast<Derived&>(*this);
    }

    constexpr Derived operator++(int) {
        Derived old = static_cast<Derived&>(*this);
        ++(*this);
        return old;
    }

    constexpr bool operator<(const Derived& other) const { return m_val < other.m_val; }
    constexpr bool operator>(const Derived& other) const { return m_val > other.m_val; }
    constexpr bool operator<=(const Derived& other) const { return m_val <= other.m_val; }
    constexpr bool operator>=(const Derived& other) const { return m_val >= other.m_val; }
    constexpr bool operator==(const Derived& other) const { return m_val == other.m_val; };

    friend std::ostream& operator<<(std::ostream& os, const Derived& obj) {
        obj.print(os);
        return os;
    };
    friend Derived;

    static constexpr Derived first() noexcept { return Derived(Derived::FIRST); }
    static constexpr Derived last() noexcept { return Derived(Derived::LAST); }
    static constexpr Derived none() noexcept { return Derived(Derived::NONE); }
};
struct File : Coordinate<File> {
   public:
    // clang-format off
    enum : uint8_t { 
        FA, FB, FC, FD, FE, FF, FG, FH, 
        
        FIRST = FA,
        LAST  = FH,
        
        NONE = 8,
        MASK = 0b111,
        SIZE = 3,
        NB   = 8 
    };
    // clang-format on

    constexpr File(uint8_t val = NONE) : Coordinate(val) {}

    static constexpr uint8_t distance(File from, File to) { return from > to ? from - to : to - from; }

    void print(std::ostream& os) const {
        static constexpr std::string_view fileToChar = "ABCDEFGH";
        if (isValid()) {
            os << fileToChar[m_val];
        } else
            os << '?';
    }
};

struct Rank : Coordinate<Rank> {
   public:
    // clang-format off
    enum : uint8_t { 
        R1, R2, R3, R4, R5, R6, R7, R8,
        
        FIRST = R1,
        LAST  = R8,
        
        NONE = 8,
        MASK = 0b111,
        SIZE = 3,
        NB   = 8
    };
    // clang-format on

    constexpr Rank(uint8_t val = NONE) : Coordinate(val) {}

    static constexpr uint8_t distance(Rank from, Rank to) { return from > to ? from - to : to - from; }

    [[nodiscard]] constexpr bool pawnStarting(Color color) const {
        return (color == Colors::WHITE && m_val == R2) || (color == Colors::BLACK && m_val == R7);
    }
    [[nodiscard]] constexpr bool pawnPromoting(Color color) const {
        return (color == Colors::WHITE && m_val == R8) || (color == Colors::BLACK && m_val == R1);
    }

    void print(std::ostream& os) const {
        static constexpr std::string_view rankToChar = "12345678";
        if (isValid()) {
            os << rankToChar[m_val];
        } else
            os << '?';
    }
};

struct Square : Coordinate<Square> {
   public:
    // clang-format off
    enum : uint8_t {
        A1, B1, C1, D1, E1, F1, G1, H1,
        A2, B2, C2, D2, E2, F2, G2, H2,
        A3, B3, C3, D3, E3, F3, G3, H3,
        A4, B4, C4, D4, E4, F4, G4, H4,
        A5, B5, C5, D5, E5, F5, G5, H5,
        A6, B6, C6, D6, E6, F6, G6, H6,
        A7, B7, C7, D7, E7, F7, G7, H7,
        A8, B8, C8, D8, E8, F8, G8, H8,
        
        FIRST = A1,
        LAST = H8,

        NONE = 64,
        MASK = 0b111111,
        SIZE = 6,
        NB = 64
    };
    // clang-format on

    constexpr Square(uint8_t val = NONE) : Coordinate(val) {}

    [[nodiscard]] constexpr File file() const { return m_val & File::MASK; }
    [[nodiscard]] constexpr Rank rank() const { return m_val >> File::SIZE; }

    [[nodiscard]] static constexpr Square create(File file, Rank rank) { return rank << File::SIZE | file; }

    static constexpr uint8_t distance(Square from, Square to) {
        static constexpr auto table = []() constexpr {
            std::array<std::array<uint8_t, NB>, NB> t{};
            for (Square i = Square::first(); i <= Square::last(); ++i) {
                for (Square j = Square::first(); j <= Square::last(); ++j) {
                    t[i][j] = std::max(File::distance(i.file(), j.file()), Rank::distance(i.rank(), j.rank()));
                }
            }
            return t;
        }();
        return table[from][to];
    }

    void print(std::ostream& os) const {
        file().print(os);
        rank().print(os);
    }
};

#endif