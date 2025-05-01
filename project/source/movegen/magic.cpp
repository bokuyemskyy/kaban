#include "magic.hpp"

#include "piece.hpp"
#include "square.hpp"

void initMagics() {
    for (const auto piece : {PieceTypes::BISHOP, PieceTypes::ROOK}) {
        for (Square square = Squares::FIRST; square <= Squares::LAST; ++square) {
        }
    }
}