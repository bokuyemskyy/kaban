#ifndef NAVIGATION_HPP
#define NAVIGATION_HPP

#include <sys/types.h>

#include <array>
#include <cctype>
#include <cstdint>
#include <iostream>
#include <string_view>

#include "piece.hpp"

template <typename Derived>
struct Coordinate {
   public:
    constexpr void                  set(uint8_t val) { m_val = val; }
    [[nodiscard]] constexpr uint8_t value() const { return m_val; }

    [[nodiscard]] constexpr bool ok() const { return m_val <= Derived::LAST; }

    [[nodiscard]] static constexpr Derived first() { return Derived::FIRST; }
    [[nodiscard]] static constexpr Derived last() { return Derived::LAST; }
    [[nodiscard]] static constexpr Derived none() { return Derived::NONE; }

    [[nodiscard]] static constexpr uint8_t number() { return Derived::NB; }
    [[nodiscard]] static constexpr uint8_t mask() { return Derived::MASK; }
    [[nodiscard]] static constexpr uint8_t size() { return Derived::SIZE; }

    constexpr operator uint8_t() { return m_val; }

    class iterator {
       public:
        using iterator_category = std::forward_iterator_tag;
        using value_type        = Derived;
        using difference_type   = std::ptrdiff_t;
        using pointer           = const Derived*;
        using reference         = const Derived&;

        constexpr iterator(uint8_t val) : current_(val) {}

        constexpr reference operator*() const { return current_; }
        constexpr pointer   operator->() const { return &current_; }

        constexpr iterator& operator++() {
            ++current_;
            return *this;
        }

        constexpr iterator operator++(int) {
            iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        constexpr bool operator==(const iterator& other) const { return current_ == other.current_; }

        constexpr bool operator!=(const iterator& other) const { return !(*this == other); }

       private:
        Derived current_;
    };

    class range {
       public:
        [[nodiscard]] constexpr iterator begin() const { return iterator(Derived::FIRST); }
        [[nodiscard]] constexpr iterator end() const { return iterator(Derived::LAST + 1); }
    };

    [[nodiscard]] static constexpr range all() { return range{}; }

    constexpr Derived operator-(const Derived& other) const { return Derived(m_val - other.m_val); }

    constexpr Derived& operator+=(const Derived& other) {
        m_val += other.m_val;
        return static_cast<Derived&>(*this);
    }

    constexpr Derived& operator-=(const Derived& other) {
        m_val -= other.m_val;
        return static_cast<Derived&>(*this);
    }

    template <typename T>
    constexpr Derived operator+(T offset) const {
        return Derived(m_val + offset);
    }

    template <typename T>
    constexpr Derived operator-(T offset) const {
        return Derived(m_val - offset);
    }

    constexpr bool operator<(const Derived& other) const { return m_val < other.m_val; }

    constexpr bool operator>(const Derived& other) const { return m_val > other.m_val; }

    constexpr bool operator<=(const Derived& other) const { return m_val <= other.m_val; }

    constexpr bool operator>=(const Derived& other) const { return m_val >= other.m_val; }

    constexpr bool operator==(const Derived& other) const { return m_val == other.m_val; }

    constexpr bool operator!=(const Derived& other) const { return m_val != other.m_val; }

    constexpr Derived& operator++() {
        ++m_val;
        return static_cast<Derived&>(*this);
    }

    constexpr Derived operator++(int) {
        Derived old = static_cast<Derived&>(*this);
        ++(*this);
        return old;
    }

    constexpr Derived& operator--() {
        --m_val;
        return static_cast<Derived&>(*this);
    }

    constexpr Derived operator--(int) {
        Derived old = static_cast<Derived&>(*this);
        --(*this);
        return old;
    }

    friend std::ostream& operator<<(std::ostream& os, const Derived& obj) {
        obj.print(os);
        return os;
    }

    friend Derived;

   private:
    uint8_t m_val;

    constexpr Coordinate(uint8_t val = none()) : m_val(val) {}
};

struct File : Coordinate<File> {
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

    static constexpr std::string_view fileToChar = "abcdefgh";

    constexpr File(uint8_t val = none()) : Coordinate(val) {}
    constexpr File(int val = none()) : Coordinate(val) {}
    explicit constexpr File(char c) {
        if (c >= 'a' && c <= 'h') {
            m_val = static_cast<uint8_t>(c - 'a');
        } else if (c >= 'A' && c <= 'H') {
            m_val = static_cast<uint8_t>(c - 'A');
        }
    }

    static constexpr uint8_t distance(File from, File to) { return from > to ? from - to : to - from; }

    void print(std::ostream& os) const {
        if (ok()) {
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

    static constexpr std::string_view rankToChar = "12345678";

    constexpr Rank(uint8_t val = none()) : Coordinate(val) {}
    constexpr Rank(int val = none()) : Coordinate(val) {}
    explicit constexpr Rank(char c) { m_val = static_cast<uint8_t>(c - '1'); }

    static constexpr uint8_t distance(Rank from, Rank to) { return from > to ? from - to : to - from; }

    [[nodiscard]] constexpr bool pawnStarting(Color color) const {
        return (color == Colors::WHITE && m_val == R2) || (color == Colors::BLACK && m_val == R7);
    }
    [[nodiscard]] constexpr bool pawnPromoting(Color color) const {
        return (color == Colors::WHITE && m_val == R8) || (color == Colors::BLACK && m_val == R1);
    }

    void print(std::ostream& os) const {
        if (ok()) {
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

    constexpr Square(uint8_t val = none()) : Coordinate(val) {}
    constexpr Square(int val = none()) : Coordinate(val) {}
    explicit constexpr Square(File file, Rank rank) : Coordinate((rank << File::size()) | file) {}
    explicit constexpr Square(std::string_view str) : Coordinate() {
        if (str.size() == 2) {
            File file(str[0]);
            Rank rank(str[1]);
            if (file.ok() && rank.ok()) {
                Square(file, rank);
            }
        }
    }

    [[nodiscard]] constexpr File file() const { return m_val & File::mask(); }
    [[nodiscard]] constexpr Rank rank() const { return m_val >> File::size(); }

    static constexpr uint8_t distance(Square from, Square to) {
        static constexpr auto table = []() constexpr {
            std::array<std::array<uint8_t, NB>, NB> t{};
            for (auto i : Square::all()) {
                for (auto j : Square::all()) {
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

struct Direction {
   public:
    enum : int8_t {
        E = 1,
        N = 8,
        W = -1,
        S = -8,

        NE = N + E,
        SE = S + E,
        NW = N + W,
        SW = S + W,

        NNE = N + N + E,
        ENE = E + E + N,
        ESE = E + E + S,
        SSE = S + S + E,
        SSW = S + S + W,
        WSW = W + W + S,
        WNW = W + W + N,
        NNW = N + N + W,

        NONE = 0
    };

    constexpr Direction(int8_t val) : m_val(val) {};

    constexpr void                 set(int8_t val) { m_val = val; }
    [[nodiscard]] constexpr int8_t value() const { return m_val; }

    constexpr operator int8_t() const { return m_val; }

   private:
    int8_t m_val;
};

constexpr std::array<Direction, 1> whitePawnPushDirections = {Direction::N};

constexpr std::array<Direction, 2> whitePawnAttackDirections = {Direction::NE, Direction::NW};

constexpr std::array<Direction, 8> knightDirections = {Direction::NNE, Direction::NNW, Direction::ENE, Direction::WNW,
                                                       Direction::SSE, Direction::SSW, Direction::ESE, Direction::WSW};

constexpr std::array<Direction, 4> bishopDirections = {Direction::NE, Direction::NW, Direction::SE, Direction::SW};

constexpr std::array<Direction, 4> rookDirections = {Direction::N, Direction::S, Direction::E, Direction::W};

constexpr std::array<Direction, 8> queenDirections = {Direction::NE, Direction::NW, Direction::SE, Direction::SW,
                                                      Direction::N,  Direction::S,  Direction::E,  Direction::W};

constexpr std::array<Direction, 8> kingDirections = {Direction::E,  Direction::N,  Direction::W,  Direction::S,
                                                     Direction::NE, Direction::NW, Direction::SE, Direction::SW};

#endif