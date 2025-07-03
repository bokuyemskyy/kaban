#include "bitboard.hpp"
#include "movegen.hpp"
#include "square.hpp"

inline void initEngine() {
    Squares::init();
    Bitboards::init();
    Movegen::init();
}