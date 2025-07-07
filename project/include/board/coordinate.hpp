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
    constexpr void set(uint8_t v) { m_val = v; }

    [[nodiscard]] constexpr uint8_t& value() { return m_val; }
    [[nodiscard]] constexpr uint8_t  value() const { return m_val; }

    [[nodiscard]] constexpr bool ok() const { return m_val <= static_cast<uint8_t>(Derived::Value::LAST); }

    [[nodiscard]] static constexpr Derived first() { return Derived(Derived::Value::FIRST); }
    [[nodiscard]] static constexpr Derived last() { return Derived(Derived::Value::LAST); }
    [[nodiscard]] static constexpr Derived none() { return Derived(Derived::Value::NONE); }

    [[nodiscard]] static constexpr uint8_t number() { return static_cast<uint8_t>(Derived::Value::NB); }
    [[nodiscard]] static constexpr uint8_t mask() { return static_cast<uint8_t>(Derived::Value::MASK); }
    [[nodiscard]] static constexpr uint8_t size() { return static_cast<uint8_t>(Derived::Value::SIZE); }

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
            ++current_.value();
            return *this;
        }

        constexpr iterator operator++(int) {
            iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        constexpr bool operator==(const iterator& other) const { return current_.value() == other.current_.value(); }

        constexpr bool operator!=(const iterator& other) const { return !(*this == other); }

       private:
        Derived current_;
    };

    class range {
       public:
        constexpr iterator begin() const { return iterator(static_cast<uint8_t>(Derived::Value::FIRST)); }
        constexpr iterator end() const { return iterator(static_cast<uint8_t>(Derived::Value::LAST) + 1); }
    };

    [[nodiscard]] static constexpr range all() { return range{}; }
    constexpr Derived                    operator-(const Derived& other) const {
        return Derived(static_cast<const Derived*>(this)->value() - other.value());
    }

    constexpr Derived& operator+=(const Derived& other) {
        static_cast<Derived*>(this)->value() += other.value();
        return static_cast<Derived&>(*this);
    }

    constexpr Derived& operator-=(const Derived& other) {
        static_cast<Derived*>(this)->value() -= other.value();
        return static_cast<Derived&>(*this);
    }

    template <typename T>
    constexpr Derived operator+(T offset) const {
        return Derived(static_cast<const Derived*>(this)->value() + offset);
    }

    template <typename T>
    constexpr Derived operator-(T offset) const {
        return Derived(static_cast<const Derived*>(this)->value() - offset);
    }

    constexpr bool operator<(const Derived& other) const {
        return static_cast<const Derived*>(this)->value() < other.value();
    }

    constexpr bool operator>(const Derived& other) const {
        return static_cast<const Derived*>(this)->value() > other.value();
    }

    constexpr bool operator<=(const Derived& other) const {
        return static_cast<const Derived*>(this)->value() <= other.value();
    }

    constexpr bool operator>=(const Derived& other) const {
        return static_cast<const Derived*>(this)->value() >= other.value();
    }

    constexpr bool operator==(const Derived& other) const {
        return static_cast<const Derived*>(this)->value() == other.value();
    }

    constexpr bool operator!=(const Derived& other) const {
        return static_cast<const Derived*>(this)->value() != other.value();
    }

    constexpr Derived& operator++() {
        ++static_cast<Derived*>(this)->value();
        return static_cast<Derived&>(*this);
    }

    constexpr Derived operator++(int) {
        Derived old = static_cast<Derived&>(*this);
        ++(*this);
        return old;
    }

    constexpr Derived& operator--() {
        --static_cast<Derived*>(this)->value();
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

    constexpr Coordinate(uint8_t val) : m_val(val) {}
};

struct File : Coordinate<File> {
    // clang-format off
    enum class Value : uint8_t { 
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

    explicit constexpr File(uint8_t val = none().value()) : Coordinate(val) {}
    explicit constexpr File(int val = none().value()) : Coordinate(val) {}
    constexpr File(Value val = static_cast<Value>(none().value())) : Coordinate(static_cast<uint8_t>(val)) {}
    explicit constexpr File(char c) : Coordinate(none().value()) {
        if (c >= 'a' && c <= 'h') {
            m_val = static_cast<uint8_t>(c - 'a');
        } else if (c >= 'A' && c <= 'H') {
            m_val = static_cast<uint8_t>(c - 'A');
        }
    }

    static constexpr uint8_t distance(File from, File to) {
        return from > to ? from.value() - to.value() : to.value() - from.value();
    }

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
    enum Value : uint8_t { 
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

    explicit constexpr Rank(uint8_t val = none().value()) : Coordinate(val) {}
    explicit constexpr Rank(int val = none().value()) : Coordinate(val) {}
    constexpr Rank(Value val = static_cast<Value>(none().value())) : Coordinate(static_cast<uint8_t>(val)) {}
    explicit constexpr Rank(char c) : Coordinate(none().value()) { m_val = static_cast<uint8_t>(c - '1'); }

    static constexpr uint8_t distance(Rank from, Rank to) {
        return from > to ? from.value() - to.value() : to.value() - from.value();
    }

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
    enum Value : uint8_t {
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

    explicit constexpr Square(uint8_t val = none().value()) : Coordinate(val) {}
    explicit constexpr Square(int val = none().value()) : Coordinate(val) {}
    constexpr Square(Value val = static_cast<Value>(none().value())) : Coordinate(static_cast<uint8_t>(val)) {}
    explicit constexpr Square(File file, Rank rank) : Coordinate((rank.value() << File::size()) | file.value()) {}
    explicit constexpr Square(std::string_view str) : Coordinate(none().value()) {
        if (str.size() == 2) {
            File file(str[0]);
            Rank rank(str[1]);
            if (file.ok() && rank.ok()) {
                Square(file, rank);
            }
        }
    }

    [[nodiscard]] constexpr File file() const { return File(m_val & File::mask()); }
    [[nodiscard]] constexpr Rank rank() const { return Rank(m_val >> File::size()); }

    static constexpr uint8_t distance(Square from, Square to) {
        static constexpr auto table = []() constexpr {
            std::array<std::array<uint8_t, NB>, NB> t{};
            for (Square i = Square::first(); i <= Square::last(); ++i) {
                for (Square j = Square::first(); j <= Square::last(); ++j) {
                    t[i.value()][j.value()] =
                        std::max(File::distance(i.file(), j.file()), Rank::distance(i.rank(), j.rank()));
                }
            }
            return t;
        }();
        return table[from.value()][to.value()];
    }

    void print(std::ostream& os) const {
        file().print(os);
        rank().print(os);
    }
};

#endif