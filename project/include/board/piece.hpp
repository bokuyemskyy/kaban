#pragma once

#include <cassert>
#include <cstdint>
#include <ostream>
#include <string_view>

#include "iterator.hpp"

struct PieceType : Iterable<PieceType> {
   public:
    enum : uint8_t {
        PAWN   = 0b000,
        KNIGHT = 0b001,
        BISHOP = 0b010,
        ROOK   = 0b011,
        QUEEN  = 0b100,
        KING   = 0b101,

        FIRST = PAWN,
        LAST  = KING,

        MASK = 0b111,
        SIZE = 3,
        NB   = 6
    };

    constexpr PieceType(uint8_t value) : m_value(value) {}

    [[nodiscard]] constexpr uint8_t value() const { return m_value; }

    [[nodiscard]] static constexpr uint8_t number() { return NB; }
    [[nodiscard]] static constexpr uint8_t mask() { return MASK; }
    [[nodiscard]] static constexpr uint8_t size() { return SIZE; }

    constexpr operator uint8_t() { return m_value; }

   private:
    uint8_t m_value;
};

struct Color : Iterable<Color> {
   public:
    enum : uint8_t {
        WHITE = 0,
        BLACK = 1,

        MASK = 0b1,
        SIZE = 1,
        NB   = 2
    };

    constexpr Color(uint8_t value) : m_value(value) {}

    [[nodiscard]] constexpr uint8_t        value() const { return m_value; }
    [[nodiscard]] static constexpr uint8_t number() { return NB; }
    [[nodiscard]] static constexpr uint8_t mask() { return MASK; }
    [[nodiscard]] static constexpr uint8_t size() { return SIZE; }

    constexpr operator uint8_t() { return m_value; }

   private:
    uint8_t m_value;
};

struct Piece {
    enum : uint8_t {
        W_PAWN   = (Color::WHITE << PieceType::size()) | PieceType::PAWN,
        W_KNIGHT = (Color::WHITE << PieceType::size()) | PieceType::KNIGHT,
        W_BISHOP = (Color::WHITE << PieceType::size()) | PieceType::BISHOP,
        W_ROOK   = (Color::WHITE << PieceType::size()) | PieceType::ROOK,
        W_QUEEN  = (Color::WHITE << PieceType::size()) | PieceType::QUEEN,
        W_KING   = (Color::WHITE << PieceType::size()) | PieceType::KING,

        B_PAWN   = (Color::BLACK << PieceType::size()) | PieceType::PAWN,
        B_KNIGHT = (Color::BLACK << PieceType::size()) | PieceType::KNIGHT,
        B_BISHOP = (Color::BLACK << PieceType::size()) | PieceType::BISHOP,
        B_ROOK   = (Color::BLACK << PieceType::size()) | PieceType::ROOK,
        B_QUEEN  = (Color::BLACK << PieceType::size()) | PieceType::QUEEN,
        B_KING   = (Color::BLACK << PieceType::size()) | PieceType::KING,

        FIRST = W_PAWN,
        LAST  = B_KING,

        NONE = 0b1111,
        MASK = 0b1111,
        SIZE = 4,
        NB   = 12
    };

    static constexpr std::string_view pieceToChar = "PNBRQK  pnbrqk";

    constexpr Piece(uint8_t value = none()) : m_value(value) {}
    explicit constexpr Piece(PieceType pieceType, Color color) : m_value((color << PieceType::size()) | pieceType) {}

    explicit constexpr Piece(char c) {
        size_t index = pieceToChar.find(c);
        m_value      = (index != std::string_view::npos) ? static_cast<uint8_t>(index) : 0;
    }

    [[nodiscard]] constexpr uint8_t value() const { return m_value; }

    [[nodiscard]] constexpr bool ok() const { return m_value <= LAST && m_value != 6 && m_value != 7; }

    [[nodiscard]] static constexpr Piece none() { return Piece::NONE; }

    [[nodiscard]] constexpr Color color() const { return m_value >> PieceType::size(); }
    [[nodiscard]] constexpr Color pieceType() const { return m_value & PieceType::mask(); }

    [[nodiscard]] static constexpr uint8_t number() { return NB; }
    [[nodiscard]] static constexpr uint8_t mask() { return MASK; }
    [[nodiscard]] static constexpr uint8_t size() { return SIZE; }

    [[nodiscard]] static constexpr Piece first() { return Piece::FIRST; }
    [[nodiscard]] static constexpr Piece last() { return Piece::LAST; }

    class Iterator {
       private:
        uint8_t        m_current;
        constexpr void skipInvalid() {
            while (m_current <= Piece::LAST && !Piece(m_current).ok()) {
                ++m_current;
            }
        }

       public:
        using iterator_category = std::forward_iterator_tag;
        using value_type        = Piece;
        using difference_type   = std::ptrdiff_t;
        using pointer           = const Piece*;
        using reference         = const Piece&;

        constexpr Iterator(uint8_t value) : m_current(value) { skipInvalid(); }

        constexpr Piece operator*() const { return m_current; }

        constexpr Iterator& operator++() {
            ++m_current;
            skipInvalid();
            return *this;
        }

        constexpr Iterator operator++(int) {
            Iterator tmp = *this;
            ++m_current;
            skipInvalid();
            return tmp;
        }

        constexpr bool operator==(const Iterator& other) const { return m_current == other.m_current; }

        constexpr bool operator!=(const Iterator& other) const { return m_current != other.m_current; }
    };

    class Range {
       public:
        [[nodiscard]] constexpr Iterator begin() const { return Piece::FIRST; }
        [[nodiscard]] constexpr Iterator end() const { return Piece::LAST + 1; }
    };

    [[nodiscard]] static constexpr Range all() { return Range{}; }

    constexpr operator uint8_t() { return m_value; }

    friend std::ostream& operator<<(std::ostream& os, const Piece& obj) {
        obj.print(os);
        return os;
    }

    void print(std::ostream& os) const { os << to_char(); }

    [[nodiscard]] char constexpr to_char() const { return ok() ? pieceToChar[m_value] : '?'; }

   private:
    uint8_t m_value;
};
