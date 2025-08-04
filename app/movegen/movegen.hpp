#pragma once

#include <array>
#include <cstddef>
#include <vector>

#include "bitboard.hpp"
#include "navigation.hpp"
#include "piece.hpp"
#include "position.hpp"

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
    static constexpr Bitboard knightAttackBB(Square square) {
        static constexpr auto table = []() constexpr {
            std::array<Bitboard, Square::number()> t{};

            constexpr std::array<Direction, 8> knightDirections = {Direction::NNE, Direction::NNW, Direction::ENE,
                                                                   Direction::WNW, Direction::SSE, Direction::SSW,
                                                                   Direction::ESE, Direction::WSW};

            for (auto square_ : Square::all()) {
                Bitboard attackBB = Bitboard::zero();
                for (const auto& direction : knightDirections) {
                    attackBB |= Bitboard::destination(square_, direction);
                }
                t[square_] = attackBB;
            }

            return t;
        }();

        return table[square];
    }

    static constexpr Bitboard kingAttackBB(Square square) {
        static constexpr auto table = []() constexpr {
            std::array<Bitboard, Square::number()> t{};

            constexpr std::array<Direction, 8> kingDirections = {Direction::E,  Direction::N,  Direction::W,
                                                                 Direction::S,  Direction::NE, Direction::NW,
                                                                 Direction::SE, Direction::SW};

            for (auto square_ : Square::all()) {
                Bitboard attackBB = Bitboard::zero();
                for (const auto& direction : kingDirections) {
                    attackBB |= Bitboard::destination(square_, direction);
                }
                t[square_] = attackBB;
            }

            return t;
        }();

        return table[square];
    }

    const int BISHOP_ATTACK_NB = 0x1480;
    const int ROOK_ATTACK_NB   = 0x19000;

    // extern std::vector<Bitboard> bishopAttacks;
    // extern std::vector<Bitboard> rookAttacks;

    void init();

    template <GenerationType GT>
    size_t generateMoves(const Position& position, std::array<Move, MAX_MOVES>& /*moveList*/) {
        Bitboard pieces = position.bitboard(position.turn(), PieceType::PAWN);

        while (!pieces.empty()) {
            // for each pawn we do poplsb and generate moves
        }

        pieces = position.bitboard(position.turn(), PieceType::KNIGHT);
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