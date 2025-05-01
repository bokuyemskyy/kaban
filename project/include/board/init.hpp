#include "bitboard.hpp"
#include "magic.hpp"
#include "movegen.hpp"
#include "square.hpp"

inline void initEngine() {
    initSquares();
    initBitboards();
    initMagics();
    initMovegen();
}