#include "movegen.hpp"

// MOVEGEN IS UNDER RECONSTRUCTION
// MOVEGEN IS UNDER RECONSTRUCTION
// MOVEGEN IS UNDER RECONSTRUCTION

template <>
constexpr Bitboard Movegen::attackBB<PieceType::KNIGHT>(Square square) {
    constexpr std::array<Direction, 8> knightDirections = {Direction::NNE, Direction::NNW, Direction::ENE,
                                                           Direction::WNW, Direction::SSE, Direction::SSW,
                                                           Direction::ESE, Direction::WSW};

    static constexpr auto table = simpleAttackTable(knightDirections);
    return table[square];
}
template <>
constexpr Bitboard Movegen::attackBB<PieceType::KING>(Square square) {
    constexpr std::array<Direction, 8> kingDirections = {Direction::E,  Direction::N,  Direction::W,  Direction::S,
                                                         Direction::NE, Direction::NW, Direction::SE, Direction::SW};

    static constexpr auto table = simpleAttackTable(kingDirections);
    return table[square];
}

//
//
//

/*size_t generatePseudoLegalMoves(std::vector<Move>* moveStack) {
    size_t prevBufferSize = moveBuffer->size();

    Bitboard pieces = Bitboard::zero();

    pieces = position.m_color_bb[m_turn] & m_piece_type_bb[PieceType::PAWN];
    while (pieces != Bitboard::zero()) {
        Square   square     = poplsb(pieces);
        Bitboard singlePush = Movegen::pawnSinglePushes[m_turn][square];

        if ((singlePush & m_occupancy) == Bitboard::zero()) {
            if (isPawnStartingRank(getRank(square), m_turn)) {
                Bitboard doublePush = m_turn == Color::WHITE ? (singlePush << 8) : (singlePush >> 8);

                if ((doublePush & m_occupancy) == Bitboard::zero()) {
                    moveBuffer.emplace_back(createMove(square, poplsb(doublePush), 0));
                }
            }
            moveBuffer.emplace_back(createMove(square, poplsb(singlePush), 0));
        }

        Bitboard attacksBB = Movegen::pawnAttacks[m_turn][square] & m_color_bb[!m_turn];

        while (attacksBB != Bitboard::zero()) {
            moveBuffer.emplace_back(createMove(square, poplsb(attacksBB), 0));
        }
    }

    pieces = m_color_bb[m_turn] & m_piece_type_bb[PieceType::KNIGHT];
    while (pieces != Bitboard::zero()) {
        Square   square    = poplsb(pieces);
        Bitboard attacksBB = Movegen::getMovesBB<PieceType::KNIGHT>(this, square) & ~m_color_bb[m_turn];
        while (attacksBB != Bitboard::zero()) {
            moveBuffer.emplace_back(createMove(square, poplsb(attacksBB), 0));
        }
    }
    pieces = m_color_bb[m_turn] & m_piece_type_bb[PieceType::BISHOP];
    while (pieces != Bitboard::zero()) {
        Square   square    = poplsb(pieces);
        Bitboard attacksBB = Movegen::getMovesBB<PieceType::BISHOP>(square)
                                 Movegen::bishopAttacks[square][Movegen::getIndexOfOccupancy<PieceType::BISHOP>(
                                     square, m_occupancy)] &
                             ~m_color_bb[m_turn];
        while (attacksBB != Bitboard::zero()) {
            moveBuffer.emplace_back(createMove(square, poplsb(attacksBB), 0));
        }
    }
    pieces = m_color_bb[m_turn] & m_piece_type_bb[PieceType::ROOK];
    while (pieces != Bitboard::zero()) {
        Square   square = poplsb(pieces);
        Bitboard attacksBB =
            Movegen::rookAttacks[square][Movegen::getIndexOfOccupancy<PieceType::ROOK>(square, m_occupancy)] &
            ~m_color_bb[m_turn];
        while (attacksBB != Bitboard::zero()) {
            moveBuffer.emplace_back(createMove(square, poplsb(attacksBB), 0));
        }
    }

    pieces = m_color_bb[m_turn] & m_piece_type_bb[PieceType::QUEEN];
    while (pieces != Bitboard::zero()) {
        Square   square = poplsb(pieces);
        Bitboard attacksBB =
            (Movegen::rookAttacks[square][Movegen::getIndexOfOccupancy<PieceType::ROOK>(square, m_occupancy)] |
             Movegen::bishopAttacks[square][Movegen::getIndexOfOccupancy<PieceType::BISHOP>(square, m_occupancy)]) &
            ~m_color_bb[m_turn];
        while (attacksBB != Bitboard::zero()) {
            moveBuffer.emplace_back(createMove(square, poplsb(attacksBB), 0));
        }
    }

    pieces = m_color_bb[m_turn] & m_piece_type_bb[PieceType::KING];
    while (pieces != 0ULL) {
        Square   square    = poplsb(pieces);
        Bitboard attacksBB = Movegen::pseudoAttacks[PieceType::KING][square] & ~m_color_bb[m_turn];
        while (attacksBB != Bitboard::zero()) {
            moveBuffer.emplace_back(createMove(square, poplsb(attacksBB), 0));
        }
    }
    return moveBuffer.size() - prevBufferSize;
     return 0;
}*/

bool isLegal() {
    /*Square king = lsb(m_color_bb[m_them] & m_piece_type_bb[PieceType::KING]);

    Bitboard enemyAttackers = Bitboard::zero();

    enemyAttackers |= Movegen::pawnAttacks[m_them][king] & (m_color_bb[m_us] & m_piece_type_bb[PieceType::PAWN]);
    enemyAttackers |=
        Movegen::pseudoAttacks[PieceType::KING][king] & (m_color_bb[m_us] & m_piece_type_bb[PieceType::KING]);
    enemyAttackers |=
        Movegen::pseudoAttacks[PieceType::KNIGHT][king] & (m_color_bb[m_us] & m_piece_type_bb[PieceType::KNIGHT]);

    enemyAttackers |= Movegen::bishopAttacks[king][Movegen::getIndexOfOccupancy<PieceType::BISHOP>(king,
    m_occupancy)] & (m_color_bb[m_us] & (m_piece_type_bb[PieceType::BISHOP] | m_piece_type_bb[PieceType::QUEEN]));
    enemyAttackers |= Movegen::rookAttacks[king][Movegen::getIndexOfOccupancy<PieceType::ROOK>(king, m_occupancy)] &
                      (m_color_bb[m_us] & (m_piece_type_bb[PieceType::ROOK] | m_piece_type_bb[PieceType::QUEEN]));

    return enemyAttackers == Bitboard::zero();
*/
    return false;
}

/*
template <PieceType PieceType>
Bitboard getMovesBB(Position& position, Square square) {
    if constexpr (PieceType == PieceType::ROOK)
        return rook_attacks(square);
    else if constexpr (PieceType == PieceType::BISHOP)
        return bishop_attacks(square);
    else if constexpr (PieceType == PieceType::QUEEN)
        return rook_attacks(square) | bishop_attacks(square);
    else if constexpr (PieceType == PieceType::KNIGHT)
        return pseudoAttacks[PieceType::KNIGHT][square];
    else if constexpr (PieceType == PieceType::KING)
        return king_attacks(square);
    else if constexpr (PieceType == PieceType::PAWN)
        return pawn_attacks(square);  // You might need side info here
    else
        static_assert(false, "Unsupported piece type");
}
*/
