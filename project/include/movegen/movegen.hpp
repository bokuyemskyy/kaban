#ifndef MOVEGEN_HPP
#define MOVEGEN_HPP

#include <array>
#include <iostream>

#include "bitboard.hpp"
#include "position.hpp"

namespace Movegen {

constexpr uint8_t MAX_MOVES = 256;  // limit of possible number of moves that can be performed from any position

extern std::array<std::array<Bitboard, Squares::NB>, PieceTypes::NB> pseudoAttacks;

extern std::array<std::array<Bitboard, Squares::NB>, Colors::NB> pawnAttacks;
extern std::array<std::array<Bitboard, Squares::NB>, Colors::NB> pawnSinglePushes;

const int BISHOP_ATTACK_NB = 0x1480;
const int ROOK_ATTACK_NB   = 0x19000;

extern std::vector<Bitboard> bishopAttacks;
extern std::vector<Bitboard> rookAttacks;

template <PieceType PieceType>
Bitboard getMovesBB(Square square);

void init();

size_t generatePseudoLegalMoves(std::vector<Move>& moveStack);

template <bool Root>
int perft(Position& position, int depth, std::vector<Move>& moveStack) {
    const size_t start = moveStack.size();
    const size_t count = generatePseudoLegalMoves(moveStack);

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
                    std::cout << squareToString(getFrom(move)) << squareToString(getTo(move)) << ": " << subnodes
                              << '\n';
                }
                nodes += subnodes;
            }
        }
        position.undoMove();
    }
    moveStack.resize(start);
    return nodes;
}

int perft(Position& position, int depth, bool verbose = true) {
    std::vector<Move> moveStack;
    if (verbose) {
        return perft<true>(position, depth, moveStack);
    } else {
        return perft<false>(position, depth, moveStack);
    }
}

}  // namespace Movegen

#endif