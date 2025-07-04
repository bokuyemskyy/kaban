#ifndef SQUARE_HPP
#define SQUARE_HPP

#include <netinet/in.h>

#include <array>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string_view>

#include "piece.hpp"

struct File {
   private:
    uint8_t m_val;

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

    constexpr File(uint8_t val = NONE) : m_val(val) {};

    constexpr operator uint8_t() const { return m_val; }

    constexpr File& operator++() {
        ++m_val;
        return *this;
    }
    constexpr File operator++(int) {
        File old = *this;
        ++(*this);
        return old;
    }

    static constexpr uint8_t distance(File from, File to) { return abs(from - to); }

    friend std::ostream& operator<<(std::ostream& os, const File& file) {
        static constexpr std::string_view fileToChar = "ABCDEFGH";
        if (file <= File::LAST) {
            os << fileToChar[file];
        } else
            os << '?';
        return os;
    }
};

struct Rank {
   private:
    uint8_t m_val;

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

    constexpr Rank(uint8_t val = NONE) : m_val(val) {}

    constexpr operator uint8_t() const { return m_val; }

    constexpr Rank& operator++() {
        ++m_val;
        return *this;
    }
    constexpr Rank operator++(int) {
        Rank old = *this;
        ++(*this);
        return old;
    }

    static constexpr uint8_t distance(Rank from, Rank to) { return abs(from - to); }

    [[nodiscard]] constexpr bool pawnStarting(Color color) const {
        return (color == Colors::WHITE && m_val == R2) || (color == Colors::BLACK && m_val == R7);
    }
    [[nodiscard]] constexpr bool pawnPromoting(Color color) const {
        return (color == Colors::WHITE && m_val == R8) || (color == Colors::BLACK && m_val == R1);
    }

    friend std::ostream& operator<<(std::ostream& os, const Rank& rank) {
        static constexpr std::string_view fileToChar = "12345678";
        if (rank <= Rank::LAST) {
            os << fileToChar[rank];
        } else
            os << '?';
        return os;
    }
};

struct Square {
   private:
    uint8_t m_val;

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

    constexpr Square(uint8_t val = NONE) : m_val(val) {}

    constexpr operator uint8_t() const { return m_val; }

    constexpr Square& operator++() {
        ++m_val;
        return *this;
    }
    constexpr Square operator++(int) {
        Square old = *this;
        ++(*this);
        return old;
    }

    [[nodiscard]] constexpr File file() const { return m_val & File::MASK; }
    [[nodiscard]] constexpr Rank rank() const { return m_val >> File::SIZE; }

    [[nodiscard]] static constexpr Square create(File file, Rank rank) { return rank << File::SIZE | file; }

    static uint8_t distance(Square from, Square to) {
        static auto table = [] {
            std::array<std::array<uint8_t, NB>, NB> t{};
            for (Square i = Square::FIRST; i <= Square::LAST; ++i) {
                for (Square j = Square::FIRST; j <= Square::LAST; ++j) {
                    t[i][j] = std::max(abs(i.file() - j.file()), abs(i.rank() - j.rank()));
                }
            }
            return t;
        }();
        return table[from][to];
    }

    [[nodiscard]] constexpr bool isValid() const { return m_val <= Square::LAST; }

    friend std::ostream& operator<<(std::ostream& os, const Square& square) {
        os << square.file() << square.rank();
        return os;
    }
};

#endif