#include "square.hpp"

std::array<std::array<uint8_t, Squares::NB>, Squares::NB> squareDistance{};

void initSquares() {
    for (Square from = Squares::FIRST; from <= Squares::LAST; ++from) {
        for (Square to = Squares::FIRST; to <= Squares::LAST; ++to) {
            squareDistance[from][to] = std::max(abs(getFile(from) - getFile(to)), abs(getRank(from) - getRank(to)));
        }
    }
}