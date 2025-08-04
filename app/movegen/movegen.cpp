#include "movegen.hpp"

// MOVEGEN IS UNDER RECONSTRUCTION
// MOVEGEN IS UNDER RECONSTRUCTION
// MOVEGEN IS UNDER RECONSTRUCTION

/*Bitboard getRookAttacks(Square square, Bitboard occupancy) {
    Bitboard attackBB = Bitboard::zero();
    for (const auto& direction : rookDirections) {
        for (uint8_t i = 1;; ++i) {
            Bitboard destination = Bitboard::destinationBB(square + (direction * (i - 1)), direction);
            if (destination == Bitboard::zero()) break;

            attackBB |= destination;

            if (occupancy & destination) break;
        }
    }
    return attackBB;
}*/
/*Bitboard getBishopAttacks(Square square, Bitboard occupancy) {
    Bitboard attackBB = Bitboard::zero();
    for (const auto& direction : bishopDirections) {
        for (uint8_t i = 1;; ++i) {
            Bitboard destination = Bitboard::destinationBB(square + (direction * (i - 1)), direction);
            if (destination == Bitboard::zero()) break;

            attackBB |= destination;

            if (occupancy & destination) break;
        }
    }
    return attackBB;
}*/

void init() {
    // initMagics(PieceType::BISHOP);
    // initMagics(PieceType::ROOK);

    /*auto bishopPremaskOccupancies = getPremaskOccupancies(PieceType::BISHOP, square);
    for (const auto& occupancy : bishopPremaskOccupancies) {
        bishopAttacks[square][getIndexOfOccupancy<PieceType::BISHOP>(square, occupancy)] =
            getBishopAttacks(square, occupancy);
    }

    auto rookPremaskOccupancies = getPremaskOccupancies(PieceType::ROOK, square);
    for (const auto& occupancy : rookPremaskOccupancies) {
        rookAttacks[square][getIndexOfOccupancy<PieceType::ROOK>(square, occupancy)] =
            getRookAttacks(square, occupancy);
    }*/
}

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
