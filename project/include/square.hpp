#ifndef SQUARE_HPP
#define SQUARE_HPP

#include <string>

#include "constants.hpp"
#include "piece.hpp"
#include "types.hpp"

// clang-format off

enum class Square : u8 {
    A1, B1, C1, D1, E1, F1, G1, H1,
    A2, B2, C2, D2, E2, F2, G2, H2,
    A3, B3, C3, D3, E3, F3, G3, H3,
    A4, B4, C4, D4, E4, F4, G4, H4,
    A5, B5, C5, D5, E5, F5, G5, H5,
    A6, B6, C6, D6, E6, F6, G6, H6,
    A7, B7, C7, D7, E7, F7, G7, H7,
    A8, B8, C8, D8, E8, F8, G8, H8,
    NONE
};

enum class File : u8 { FA, FB, FC, FD, FE, FF, FG, FH, NONE };
enum class Rank : u8 { R1, R2, R3, R4, R5, R6, R7, R8, NONE };

inline File getFile(Square s) { return static_cast<File>(static_cast<u8>(s) & (0b111)); }

inline Rank getRank(Square s) { return static_cast<Rank>(static_cast<u8>(s) >> 3); }

inline Square makeSquare(File f, Rank r) {
    return static_cast<Square>(static_cast<u8>(r) << 3 | static_cast<u8>(f));
}
inline File operator+(File f, u8 offset) {
    u8 newIndex = static_cast<u8>(f) + offset;
    return (newIndex >= 0 && newIndex < BOARD_SIZE) ? static_cast<File>(newIndex) : File::NONE;
}
inline File operator-(File f, u8 offset) {
    u8 newIndex = static_cast<u8>(f) - offset;
    return (newIndex >= 0 && newIndex < BOARD_SIZE) ? static_cast<File>(newIndex) : File::NONE;
}
inline File operator+(File f, File offset) {
    u8 newIndex = static_cast<u8>(f) + static_cast<u8>(offset);
    return (newIndex >= 0 && newIndex < BOARD_SIZE) ? static_cast<File>(newIndex) : File::NONE;
}
inline File operator-(File f, File offset) {
    u8 newIndex = static_cast<u8>(f) - static_cast<u8>(offset);
    return (newIndex >= 0 && newIndex < BOARD_SIZE) ? static_cast<File>(newIndex) : File::NONE;
}
inline Rank operator+(Rank f, u8 offset) {
    u8 newIndex = static_cast<u8>(f) + offset;
    return (newIndex >= 0 && newIndex < BOARD_SIZE) ? static_cast<Rank>(newIndex) : Rank::NONE;
}
inline Rank operator-(Rank f, u8 offset) {
    u8 newIndex = static_cast<u8>(f) - offset;
    return (newIndex >= 0 && newIndex < BOARD_SIZE) ? static_cast<Rank>(newIndex) : Rank::NONE;
}
inline Rank operator+(Rank f, Rank offset) {
    u8 newIndex = static_cast<u8>(f) + static_cast<u8>(offset);
    return (newIndex >= 0 && newIndex < BOARD_SIZE) ? static_cast<Rank>(newIndex) : Rank::NONE;
}
inline Rank operator-(Rank f, Rank offset) {
    u8 newIndex = static_cast<u8>(f) - static_cast<u8>(offset);
    return (newIndex >= 0 && newIndex < BOARD_SIZE) ? static_cast<Rank>(newIndex) : Rank::NONE;
}
inline Square operator+(Square s, u8 offset) {
    return static_cast<Square>(static_cast<u8>(s) + offset);
}

inline Square operator-(Square s, u8 offset) {
    return static_cast<Square>(static_cast<u8>(s) - offset);
}

inline Square operator+(Square s, Direction d) {
    File newFile = getFile(s) + d.first;
    if (newFile == File::NONE) return Square::NONE;
    Rank newRank = getRank(s) + d.second;
    if (newRank == Rank::NONE) return Square::NONE;
    return makeSquare(newFile, newRank);
}

inline bool isPawnStartingRank(Rank r, Color c) {
    return (c == Color::WHITE && r == Rank::R2) || (c == Color::BLACK && r == Rank::R7);
}
inline bool isPawnPromotionRank(Rank r, Color c) {
    return (c == Color::WHITE && r == Rank::R8) || (c == Color::BLACK && r == Rank::R1);
}

constexpr std::string fileToChar(File f) {
    switch (f) {
        case File::FA:
            return "a";
        case File::FB:
            return "b";
        case File::FC:
            return "c";
        case File::FD:
            return "d";
        case File::FE:
            return "e";
        case File::FF:
            return "f";
        case File::FG:
            return "g";
        case File::FH:
            return "h";
        default:
            return "?";
    }
}
constexpr std::string rankToChar(Rank r) {
    switch (r) {
        case Rank::R1:
            return "1";
        case Rank::R2:
            return "2";
        case Rank::R3:
            return "3";
        case Rank::R4:
            return "4";
        case Rank::R5:
            return "5";
        case Rank::R6:
            return "6";
        case Rank::R7:
            return "7";
        case Rank::R8:
            return "8";
        default:
            return "?";
    }
}
inline std::string squareToString(Square s) {
    return fileToChar(getFile(s)) + rankToChar(getRank(s));
}

inline Direction operator*(Direction d, u8 multiplier) {
    d.first = d.first * multiplier;
    d.second = d.second * multiplier;
    return d;
}

// clang-format on

#endif