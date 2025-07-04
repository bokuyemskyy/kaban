#include "movegen.hpp"

#include <cstddef>
#include <vector>

#include "bitboard.hpp"
#include "direction.hpp"
#include "magic.hpp"
#include "piece.hpp"
#include "square.hpp"

namespace Movegen {

std::array<std::array<Bitboard, Squares::NB>, PieceTypes::NB> pseudoAttacks{};

std::array<std::array<Bitboard, Squares::NB>, Colors::NB> pawnAttacks{};
std::array<std::array<Bitboard, Squares::NB>, Colors::NB> pawnSinglePushes{};

std::vector<Bitboard> bishopAttacks(BISHOP_ATTACK_NB);
std::vector<Bitboard> rookAttacks(ROOK_ATTACK_NB);

template <GenerationType T, Color US>
size_t generateMovesByColor(const Position& position, std::array<Move, MAX_MOVES>& moveList) {}

template <GenerationType T>
size_t generateMoves(const Position& position, std::array<Move, MAX_MOVES>& moveList) {
    Color us = position.turn();

    return us == Colors::WHITE ? generateMovesByColor<T, Colors::WHITE>(position, moveList)
                               : generateMovesByColor<T, Colors::BLACK>(position, moveList);
}

template size_t generateMoves<CAPTURE>(const Position&, std::array<Move, MAX_MOVES>&);
template size_t generateMoves<NON_CAPTURE>(const Position&, std::array<Move, MAX_MOVES>&);
template size_t generateMoves<EVASION>(const Position&, std::array<Move, MAX_MOVES>&);
template size_t generateMoves<NON_EVASION>(const Position&, std::array<Move, MAX_MOVES>&);

Bitboard getRookAttacks(Square square, Bitboard occupancy) {
    Bitboard attackBB = Bitboards::ZERO;
    for (const auto& direction : rookDirections) {
        for (uint8_t i = 1;; ++i) {
            Bitboard destination = destinationBB(square + (direction * (i - 1)), direction);
            if (destination == Bitboards::ZERO) break;

            attackBB |= destination;

            if (occupancy & destination) break;
        }
    }
    return attackBB;
}
Bitboard getBishopAttacks(Square square, Bitboard occupancy) {
    Bitboard attackBB = Bitboards::ZERO;
    for (const auto& direction : bishopDirections) {
        for (uint8_t i = 1;; ++i) {
            Bitboard destination = destinationBB(square + (direction * (i - 1)), direction);
            if (destination == Bitboards::ZERO) break;

            attackBB |= destination;

            if (occupancy & destination) break;
        }
    }
    return attackBB;
}

void init() {
    initMagics(PieceTypes::BISHOP);
    initMagics(PieceTypes::ROOK);

    for (Square square = Squares::FIRST; square <= Squares::LAST; ++square) {
        Bitboard pawnBB = Bitboards::ZERO;
        for (const auto& direction : whitePawnAttackDirections) {
            pawnBB |= destinationBB(square, direction);
        }
        pawnAttacks[Colors::WHITE][square] = pawnBB;

        pawnBB = Bitboards::ZERO;
        for (const auto& direction : whitePawnAttackDirections) {
            pawnBB |= destinationBB(square, -direction);
        }
        pawnAttacks[Colors::BLACK][square] = pawnBB;

        pawnBB = Bitboards::ZERO;
        pawnBB |= destinationBB(square, whitePawnPushDirections[0]);
        pawnSinglePushes[Colors::WHITE][square] = pawnBB;

        pawnBB = Bitboards::ZERO;
        pawnBB |= destinationBB(square, -whitePawnPushDirections[0]);
        pawnSinglePushes[Colors::BLACK][square] = pawnBB;

        Bitboard knightBB = Bitboards::ZERO;
        for (const auto& direction : knightDirections) {
            knightBB |= destinationBB(square, direction);
        }
        pseudoAttacks[PieceTypes::KNIGHT][square] = knightBB;

        auto bishopPremaskOccupancies = getPremaskOccupancies(PieceTypes::BISHOP, square);
        for (const auto& occupancy : bishopPremaskOccupancies) {
            bishopAttacks[square][getIndexOfOccupancy<PieceTypes::BISHOP>(square, occupancy)] =
                getBishopAttacks(square, occupancy);
        }

        auto rookPremaskOccupancies = getPremaskOccupancies(PieceTypes::ROOK, square);
        for (const auto& occupancy : rookPremaskOccupancies) {
            rookAttacks[square][getIndexOfOccupancy<PieceTypes::ROOK>(square, occupancy)] =
                getRookAttacks(square, occupancy);
        }

        Bitboard kingBB = Bitboards::ZERO;
        for (const auto& direction : kingDirections) {
            kingBB |= destinationBB(square, direction);
        }
        pseudoAttacks[PieceTypes::KING][square] = kingBB;
    }
}
/*
template <PieceType PieceType>
Bitboard getMovesBB(Position& position, Square square) {
    if constexpr (PieceType == PieceTypes::ROOK)
        return rook_attacks(square);
    else if constexpr (PieceType == PieceTypes::BISHOP)
        return bishop_attacks(square);
    else if constexpr (PieceType == PieceTypes::QUEEN)
        return rook_attacks(square) | bishop_attacks(square);
    else if constexpr (PieceType == PieceTypes::KNIGHT)
        return pseudoAttacks[PieceTypes::KNIGHT][square];
    else if constexpr (PieceType == PieceTypes::KING)
        return king_attacks(square);
    else if constexpr (PieceType == PieceTypes::PAWN)
        return pawn_attacks(square);  // You might need side info here
    else
        static_assert(false, "Unsupported piece type");
}
*/

size_t generatePseudoLegalMoves(std::vector<Move>* moveStack) {
    size_t prevBufferSize = moveBuffer->size();

    Bitboard pieces = Bitboards::ZERO;

    pieces = position.m_colorBB[m_turn] & m_pieceTypeBB[PieceTypes::PAWN];
    while (pieces != Bitboards::ZERO) {
        Square   square     = popLSB(pieces);
        Bitboard singlePush = Movegen::pawnSinglePushes[m_turn][square];

        if ((singlePush & m_occupancy) == Bitboards::ZERO) {
            if (isPawnStartingRank(getRank(square), m_turn)) {
                Bitboard doublePush = m_turn == Colors::WHITE ? (singlePush << 8) : (singlePush >> 8);

                if ((doublePush & m_occupancy) == Bitboards::ZERO) {
                    moveBuffer.emplace_back(createMove(square, popLSB(doublePush), 0));
                }
            }
            moveBuffer.emplace_back(createMove(square, popLSB(singlePush), 0));
        }

        Bitboard attacksBB = Movegen::pawnAttacks[m_turn][square] & m_colorBB[!m_turn];

        while (attacksBB != Bitboards::ZERO) {
            moveBuffer.emplace_back(createMove(square, popLSB(attacksBB), 0));
        }
    }

    pieces = m_colorBB[m_turn] & m_pieceTypeBB[PieceTypes::KNIGHT];
    while (pieces != Bitboards::ZERO) {
        Square   square    = popLSB(pieces);
        Bitboard attacksBB = Movegen::getMovesBB<PieceTypes::KNIGHT>(this, square) & ~m_colorBB[m_turn];
        while (attacksBB != Bitboards::ZERO) {
            moveBuffer.emplace_back(createMove(square, popLSB(attacksBB), 0));
        }
    }
    pieces = m_colorBB[m_turn] & m_pieceTypeBB[PieceTypes::BISHOP];
    while (pieces != Bitboards::ZERO) {
        Square   square = popLSB(pieces);
        Bitboard attacksBB =
            Movegen::getMovesBB<PieceTypes::BISHOP>(square)
                Movegen::bishopAttacks[square][Movegen::getIndexOfOccupancy<PieceTypes::BISHOP>(square, m_occupancy)] &
            ~m_colorBB[m_turn];
        while (attacksBB != Bitboards::ZERO) {
            moveBuffer.emplace_back(createMove(square, popLSB(attacksBB), 0));
        }
    }
    pieces = m_colorBB[m_turn] & m_pieceTypeBB[PieceTypes::ROOK];
    while (pieces != Bitboards::ZERO) {
        Square   square = popLSB(pieces);
        Bitboard attacksBB =
            Movegen::rookAttacks[square][Movegen::getIndexOfOccupancy<PieceTypes::ROOK>(square, m_occupancy)] &
            ~m_colorBB[m_turn];
        while (attacksBB != Bitboards::ZERO) {
            moveBuffer.emplace_back(createMove(square, popLSB(attacksBB), 0));
        }
    }

    pieces = m_colorBB[m_turn] & m_pieceTypeBB[PieceTypes::QUEEN];
    while (pieces != Bitboards::ZERO) {
        Square   square = popLSB(pieces);
        Bitboard attacksBB =
            (Movegen::rookAttacks[square][Movegen::getIndexOfOccupancy<PieceTypes::ROOK>(square, m_occupancy)] |
             Movegen::bishopAttacks[square][Movegen::getIndexOfOccupancy<PieceTypes::BISHOP>(square, m_occupancy)]) &
            ~m_colorBB[m_turn];
        while (attacksBB != Bitboards::ZERO) {
            moveBuffer.emplace_back(createMove(square, popLSB(attacksBB), 0));
        }
    }

    pieces = m_colorBB[m_turn] & m_pieceTypeBB[PieceTypes::KING];
    while (pieces != 0ULL) {
        Square   square    = popLSB(pieces);
        Bitboard attacksBB = Movegen::pseudoAttacks[PieceTypes::KING][square] & ~m_colorBB[m_turn];
        while (attacksBB != Bitboards::ZERO) {
            moveBuffer.emplace_back(createMove(square, popLSB(attacksBB), 0));
        }
    }
    return moveBuffer.size() - prevBufferSize;
}

bool Position::isLegal() {
    Square king = lsb(m_colorBB[m_them] & m_pieceTypeBB[PieceTypes::KING]);

    Bitboard enemyAttackers = Bitboards::ZERO;

    enemyAttackers |= Movegen::pawnAttacks[m_them][king] & (m_colorBB[m_us] & m_pieceTypeBB[PieceTypes::PAWN]);
    enemyAttackers |=
        Movegen::pseudoAttacks[PieceTypes::KING][king] & (m_colorBB[m_us] & m_pieceTypeBB[PieceTypes::KING]);
    enemyAttackers |=
        Movegen::pseudoAttacks[PieceTypes::KNIGHT][king] & (m_colorBB[m_us] & m_pieceTypeBB[PieceTypes::KNIGHT]);

    enemyAttackers |=
        Movegen::bishopAttacks[king][Movegen::getIndexOfOccupancy<PieceTypes::BISHOP>(king, m_occupancy)] &
        (m_colorBB[m_us] & (m_pieceTypeBB[PieceTypes::BISHOP] | m_pieceTypeBB[PieceTypes::QUEEN]));
    enemyAttackers |= Movegen::rookAttacks[king][Movegen::getIndexOfOccupancy<PieceTypes::ROOK>(king, m_occupancy)] &
                      (m_colorBB[m_us] & (m_pieceTypeBB[PieceTypes::ROOK] | m_pieceTypeBB[PieceTypes::QUEEN]));

    return enemyAttackers == Bitboards::ZERO;
}

}  // namespace Movegen