#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <vector>

#include "bitboard.hpp"
#include "piece.hpp"
#include "position.hpp"
#include "square.hpp"
/*
class Movegen {
   public:
    template <bool Root>
    static int perft(Position& position, int depth) {
    int nodes = 0;

    for (size_t i = 0; i < count; ++i) {
        const Move& move = moveStack[start + i];
        position.doMove(move);
        if (isLegal(position)) [[likely]] {
            if (depth == 1) {
                nodes++;
            } else {
                int subnodes = perft<false>(position, depth - 1, moveStack);
                if constexpr (Root) {
                    std::cout << move.from() << move.to() << ": " << subnodes << '\n';
                }
                nodes += subnodes;
            }
        }
        position.undoMove();
    }
    moveStack.resize(start);
    return nodes;
    return 0;
}

static int perft(Position& position, int depth, bool verbose = true) {
    std::vector<Move> moveStack;
    if (verbose) {
        return perft<true>(position, depth);
    } else {
        return perft<false>(position, depth);
    }
}
}
;
*/