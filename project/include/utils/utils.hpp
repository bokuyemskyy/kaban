#ifndef UTILS_HPP
#define UTILS_HPP

#include <cmath>
#include <cstdint>
#include <string>
#include <utility>

#include "bitboard.hpp"
#include "square.hpp"

float eucledianDistance(const std::pair<float, float> &a, const std::pair<float, float> &b);

struct Dimensions {
    int width;
    int height;
};

constexpr std::string fileToChar(File file) { return {1, static_cast<char>('a' + file)}; }
constexpr std::string rankToChar(Rank rank) { return {1, static_cast<char>('1' + rank)}; }
inline std::string    squareToString(Square s) { return fileToChar(getFile(s)) + rankToChar(getRank(s)); }

constexpr Square lsb(const Bitboard &x) { return static_cast<Square>(__builtin_ctzll(x)); }
constexpr Square popLsb(Bitboard &x) {
    Square i = lsb(x);
    x &= x - 1;
    return i;
}
constexpr uint8_t popcount(Bitboard x) { return __builtin_popcountll(x); }

#endif