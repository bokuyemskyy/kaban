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

// MOVEGEN IS UNDER RECONSTRUCTION
// MOVEGEN IS UNDER RECONSTRUCTION
// MOVEGEN IS UNDER RECONSTRUCTION

static constexpr int MAX_MOVES = 256;  // limit of possible number of moves that can be performed from any position

enum class GenerationType : uint8_t {
    CAPTURE,
    NON_CAPTURE,
    EVASION,
    NON_EVASION,
    LEGAL
};

template <GenerationType GT>
class MoveList {
   private:
    std::array<Move, MAX_MOVES> moveList;
    size_t                      size;

   public:
    explicit MoveList(const Position& position) : size(generateMoves<GT>(position, moveList)) {}
};

class Movegen {
    template <size_t N>
    static constexpr std::array<Bitboard, Square::number()> simpleAttackTable(
        const std::array<Direction, N>& directions) {
        std::array<Bitboard, Square::number()> table{};

        for (auto square : Squares::all()) {
            Bitboard attackBB = Bitboards::ZERO;
            for (const auto& direction : directions) {
                Square destination = square.moved(direction);
                if (destination.hasValue()) attackBB |= Bitboard::square(destination);
            }
            table[square.value()] = attackBB;
        }

        return table;
    }

    template <uint8_t>
    static constexpr Bitboard attackBB(Square square);

    void init();

    template <GenerationType GT>
    size_t generateMoves(const Position& position, std::array<Move, MAX_MOVES>& /*moveList*/) {
        Bitboard pieces = position.ourOccupancy(PieceTypes::PAWN);

        while (pieces.hasValue()) {
            // for each pawn we do poplsb and generate moves
        }

        pieces = position.ourOccupancy(PieceTypes::KNIGHT);
        //...
    }

   public:
    template <bool Root>
    static int perft(Position& /*position*/, int /*depth*/) {
        /*int nodes = 0;

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
        return nodes;*/
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
};