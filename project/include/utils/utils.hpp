#ifndef UTILS_HPP
#define UTILS_HPP

#include <cmath>
#include <cstdint>
#include <string>
#include <utility>

#include "bitboard.hpp"
#include "move.hpp"
#include "square.hpp"

float eucledianDistance(const std::pair<float, float> &a, const std::pair<float, float> &b);

struct Dimensions {
    int width;
    int height;
};

char        fileToChar(File file);
char        rankToChar(Rank rank);
Rank        charToRank(char c);
File        charToFile(char c);
std::string squareToString(Square s);
Square      stringToSquare(const std::string &s);
std::string moveToString(Move move);
Move        stringToMove(const std::string &s);

constexpr Square lsb(const Bitboard &x) { return static_cast<Square>(__builtin_ctzll(x)); }
constexpr Square popLsb(Bitboard &x) {
    Square i = lsb(x);
    x &= x - 1;
    return i;
}
constexpr uint8_t popcount(Bitboard x) { return __builtin_popcountll(x); }

#endif