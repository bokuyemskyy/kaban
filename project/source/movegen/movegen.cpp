#include "movegen.hpp"

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

Bitboard getRookAttacks(Square square, Bitboard occupancy) {
    Bitboard attackBB = BITBOARD_ZERO;
    for (const auto& direction : rookDirections) {
        for (uint8_t i = 1;; ++i) {
            Bitboard destination = destinationBB(square + (direction * (i - 1)), direction);
            if (destination == BITBOARD_ZERO) break;

            attackBB |= destination;

            if (occupancy & destination) break;
        }
    }
    return attackBB;
}
Bitboard getBishopAttacks(Square square, Bitboard occupancy) {
    Bitboard attackBB = BITBOARD_ZERO;
    for (const auto& direction : bishopDirections) {
        for (uint8_t i = 1;; ++i) {
            Bitboard destination = destinationBB(square + (direction * (i - 1)), direction);
            if (destination == BITBOARD_ZERO) break;

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
        Bitboard pawnBB = BITBOARD_ZERO;
        for (const auto& direction : whitePawnAttackDirections) {
            pawnBB |= destinationBB(square, direction);
        }
        pawnAttacks[Colors::WHITE][square] = pawnBB;

        pawnBB = BITBOARD_ZERO;
        for (const auto& direction : whitePawnAttackDirections) {
            pawnBB |= destinationBB(square, -direction);
        }
        pawnAttacks[Colors::BLACK][square] = pawnBB;

        pawnBB = BITBOARD_ZERO;
        pawnBB |= destinationBB(square, whitePawnPushDirections[0]);
        pawnSinglePushes[Colors::WHITE][square] = pawnBB;

        pawnBB = BITBOARD_ZERO;
        pawnBB |= destinationBB(square, -whitePawnPushDirections[0]);
        pawnSinglePushes[Colors::BLACK][square] = pawnBB;

        Bitboard knightBB = BITBOARD_ZERO;
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

        Bitboard kingBB = BITBOARD_ZERO;
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

    Bitboard pieces = BITBOARD_ZERO;

    pieces = position.m_colorBB[m_turn] & m_pieceTypeBB[PieceTypes::PAWN];
    while (pieces != BITBOARD_ZERO) {
        Square   square     = popLSB(pieces);
        Bitboard singlePush = Movegen::pawnSinglePushes[m_turn][square];

        if ((singlePush & m_occupancy) == BITBOARD_ZERO) {
            if (isPawnStartingRank(getRank(square), m_turn)) {
                Bitboard doublePush = m_turn == Colors::WHITE ? (singlePush << 8) : (singlePush >> 8);

                if ((doublePush & m_occupancy) == BITBOARD_ZERO) {
                    moveBuffer.emplace_back(createMove(square, popLSB(doublePush), 0));
                }
            }
            moveBuffer.emplace_back(createMove(square, popLSB(singlePush), 0));
        }

        Bitboard attacksBB = Movegen::pawnAttacks[m_turn][square] & m_colorBB[!m_turn];

        while (attacksBB != BITBOARD_ZERO) {
            moveBuffer.emplace_back(createMove(square, popLSB(attacksBB), 0));
        }
    }

    pieces = m_colorBB[m_turn] & m_pieceTypeBB[PieceTypes::KNIGHT];
    while (pieces != BITBOARD_ZERO) {
        Square   square    = popLSB(pieces);
        Bitboard attacksBB = Movegen::getMovesBB<PieceTypes::KNIGHT>(this, square) & ~m_colorBB[m_turn];
        while (attacksBB != BITBOARD_ZERO) {
            moveBuffer.emplace_back(createMove(square, popLSB(attacksBB), 0));
        }
    }
    pieces = m_colorBB[m_turn] & m_pieceTypeBB[PieceTypes::BISHOP];
    while (pieces != BITBOARD_ZERO) {
        Square   square = popLSB(pieces);
        Bitboard attacksBB =
            Movegen::getMovesBB<PieceTypes::BISHOP>(square)
                Movegen::bishopAttacks[square][Movegen::getIndexOfOccupancy<PieceTypes::BISHOP>(square, m_occupancy)] &
            ~m_colorBB[m_turn];
        while (attacksBB != BITBOARD_ZERO) {
            moveBuffer.emplace_back(createMove(square, popLSB(attacksBB), 0));
        }
    }
    pieces = m_colorBB[m_turn] & m_pieceTypeBB[PieceTypes::ROOK];
    while (pieces != BITBOARD_ZERO) {
        Square   square = popLSB(pieces);
        Bitboard attacksBB =
            Movegen::rookAttacks[square][Movegen::getIndexOfOccupancy<PieceTypes::ROOK>(square, m_occupancy)] &
            ~m_colorBB[m_turn];
        while (attacksBB != BITBOARD_ZERO) {
            moveBuffer.emplace_back(createMove(square, popLSB(attacksBB), 0));
        }
    }

    pieces = m_colorBB[m_turn] & m_pieceTypeBB[PieceTypes::QUEEN];
    while (pieces != BITBOARD_ZERO) {
        Square   square = popLSB(pieces);
        Bitboard attacksBB =
            (Movegen::rookAttacks[square][Movegen::getIndexOfOccupancy<PieceTypes::ROOK>(square, m_occupancy)] |
             Movegen::bishopAttacks[square][Movegen::getIndexOfOccupancy<PieceTypes::BISHOP>(square, m_occupancy)]) &
            ~m_colorBB[m_turn];
        while (attacksBB != BITBOARD_ZERO) {
            moveBuffer.emplace_back(createMove(square, popLSB(attacksBB), 0));
        }
    }

    pieces = m_colorBB[m_turn] & m_pieceTypeBB[PieceTypes::KING];
    while (pieces != 0ULL) {
        Square   square    = popLSB(pieces);
        Bitboard attacksBB = Movegen::pseudoAttacks[PieceTypes::KING][square] & ~m_colorBB[m_turn];
        while (attacksBB != BITBOARD_ZERO) {
            moveBuffer.emplace_back(createMove(square, popLSB(attacksBB), 0));
        }
    }
    return moveBuffer.size() - prevBufferSize;
}

bool Position::isLegal() {
    Square king = lsb(m_colorBB[m_them] & m_pieceTypeBB[PieceTypes::KING]);

    Bitboard enemyAttackers = BITBOARD_ZERO;

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

    return enemyAttackers == BITBOARD_ZERO;
}

}  // namespace Movegen