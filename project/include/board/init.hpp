#include "bitboard.hpp"
#include "movegen.hpp"
#include "square.hpp"

inline void initEngine() {
    Bitboards::init();
    Movegen::init();
}