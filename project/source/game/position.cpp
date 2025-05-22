#include "position.hpp"

#include <imgui.h>

#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

#include "bitboard.hpp"
#include "magic.hpp"
#include "movegen.hpp"
#include "piece.hpp"
#include "square.hpp"
#include "utils.hpp"

void Position::setFromFEN(const std::string &fen) {
    resetBoard();

    std::stringstream ss(fen);
    std::string       argument;
    size_t            argument_index = 0;
    while (std::getline(ss, argument, ' ')) {
        switch (argument_index) {
            case 0: {
                int current_rank = Files::NB - 1;
                int current_file = 0;

                for (char c : argument) {
                    if (c == '/') {
                        current_rank--;
                        current_file = 0;
                    } else if (std::isdigit(c) != 0) {
                        int num = c - '0';
                        current_file += num;
                    } else {
                        Square s = createSquare(current_file, current_rank);
                        setPiece(s, charToPiece(c));
                        ++current_file;
                    }
                }
                break;
            }
            case 1:
                if (argument == "w")
                    m_turn = Colors::WHITE;
                else
                    m_turn = Colors::BLACK;
                break;
            default:
                break;
        }
        ++argument_index;
    }
}

std::string Position::toFEN() const {
    std::stringstream fen;

    for (int r = Ranks::NB - 1; r >= 0; r--) {
        int empty = 0;
        for (int f = 0; f < Files::NB; ++f) {
            Square s = createSquare(File(f), Rank(r));
            Piece  p = m_board[s];

            if (p == Pieces::NONE) {
                ++empty;
            } else {
                if (empty > 0) {
                    fen << empty;
                    empty = 0;
                }
                fen << pieceToChar(p);
            }
        }
        if (empty > 0) {
            fen << empty;
        }
        if (r > 0) fen << '/';
    }

    return fen.str();
}

void Position::setPiece(Square square, Piece piece) {
    assert(square != Squares::NONE);
    assert(piece != Pieces::NONE);

    unsetPiece(square);
    m_colorBB[getColor(piece)] |= squareBB(square);
    m_pieceTypeBB[getPieceType(piece)] |= squareBB(square);
    m_board[square] = piece;
}

void Position::unsetPiece(Square square) {
    assert(square != Squares::NONE);

    if (m_board[square] == Pieces::NONE) return;
    m_colorBB[getColor(m_board[square])] &= ~squareBB(square);
    m_pieceTypeBB[getPieceType(m_board[square])] &= ~squareBB(square);
    m_board[square] = Pieces::NONE;
}
void Position::resetBoard() {
    for (uint8_t i = 0; i < Colors::NB; ++i) {
        m_colorBB[i] = BITBOARD_ZERO;
    }
    for (uint8_t i = 0; i < PieceTypes::NB; ++i) {
        m_pieceTypeBB[i] = BITBOARD_ZERO;
    }
    for (uint8_t i = 0; i < Squares::NB; ++i) {
        m_board[i] = Pieces::NONE;
    }
    m_turn = Colors::WHITE;
}

inline void Position::toggleTurn() { m_turn = !m_turn; }

void Position::updateCachedState() {
    m_wking     = lsb(m_colorBB[Colors::WHITE] & m_pieceTypeBB[PieceTypes::KING]);
    m_bking     = lsb(m_colorBB[Colors::BLACK] & m_pieceTypeBB[PieceTypes::KING]);
    m_occupancy = m_colorBB[Colors::WHITE] | m_colorBB[Colors::BLACK];
    m_us        = m_turn;
    m_them      = !m_turn;
}

void Position::doMove(const Move move) {
    Square from = getFrom(move);
    Square to   = getTo(move);

    Bitboard moveBB = squareBB(from) | squareBB(to);

    m_deltas.emplace_back(createDelta(m_board[to], m_castling, 0, m_halfmoves));
    m_moves.emplace_back(move);

    if (m_board[to] != Pieces::NONE) {
        Bitboard maskBB = ~squareBB(to);
        m_colorBB[getColor(m_board[to])] &= maskBB;
        m_pieceTypeBB[getPieceType(m_board[to])] &= maskBB;
    }

    m_colorBB[getColor(m_board[from])] ^= moveBB;
    m_pieceTypeBB[getPieceType(m_board[from])] ^= moveBB;

    m_board[to]   = m_board[from];
    m_board[from] = Pieces::NONE;

    toggleTurn();

    updateCachedState();
}

void Position::undoMove() {
    if (m_deltas.empty() || m_moves.empty()) return;

    Delta delta = m_deltas.back();
    Move  move  = m_moves.back();

    m_deltas.pop_back();
    m_moves.pop_back();

    Square from = getFrom(move);
    Square to   = getTo(move);

    Bitboard moveBB = squareBB(from) | squareBB(to);

    Piece movedPiece    = m_board[to];
    Piece capturedPiece = getCaptured(delta);

    m_board[from] = movedPiece;
    m_board[to]   = capturedPiece;

    m_colorBB[getColor(movedPiece)] ^= moveBB;
    m_pieceTypeBB[getPieceType(movedPiece)] ^= moveBB;

    if (capturedPiece != Pieces::NONE) {
        Bitboard toBB = squareBB(to);
        m_colorBB[getColor(capturedPiece)] |= toBB;
        m_pieceTypeBB[getPieceType(capturedPiece)] |= toBB;
    }

    m_castling  = getCastling(delta);
    m_halfmoves = getHalfmoves(delta);

    toggleTurn();

    updateCachedState();
}

size_t Position::generatePseudoLegalMoves(std::vector<Move> &moveBuffer) {
    size_t prevBufferSize = moveBuffer.size();

    Bitboard pieces = BITBOARD_ZERO;

    pieces = m_colorBB[m_turn] & m_pieceTypeBB[PieceTypes::PAWN];
    while (pieces != BITBOARD_ZERO) {
        Square   square     = popLsb(pieces);
        Bitboard singlePush = pawnSinglePushes[m_turn][square];

        if ((singlePush & m_occupancy) == BITBOARD_ZERO) {
            if (isPawnStartingRank(getRank(square), m_turn)) {
                Bitboard doublePush = m_turn == Colors::WHITE ? (singlePush << 8) : (singlePush >> 8);

                if ((doublePush & m_occupancy) == BITBOARD_ZERO) {
                    moveBuffer.emplace_back(createMove(square, popLsb(doublePush), 0));
                }
            }
            moveBuffer.emplace_back(createMove(square, popLsb(singlePush), 0));
        }

        Bitboard attacksBB = pawnAttacks[m_turn][square] & m_colorBB[!m_turn];

        while (attacksBB != BITBOARD_ZERO) {
            moveBuffer.emplace_back(createMove(square, popLsb(attacksBB), 0));
        }
    }

    pieces = m_colorBB[m_turn] & m_pieceTypeBB[PieceTypes::KNIGHT];
    while (pieces != BITBOARD_ZERO) {
        Square   square    = popLsb(pieces);
        Bitboard attacksBB = pseudoAttacks[PieceTypes::KNIGHT][square] & ~m_colorBB[m_turn];
        while (attacksBB != BITBOARD_ZERO) {
            moveBuffer.emplace_back(createMove(square, popLsb(attacksBB), 0));
        }
    }
    pieces = m_colorBB[m_turn] & m_pieceTypeBB[PieceTypes::BISHOP];
    while (pieces != BITBOARD_ZERO) {
        Square   square = popLsb(pieces);
        Bitboard attacksBB =
            bishopAttacks[square][getIndexOfOccupancy<PieceTypes::BISHOP>(square, m_occupancy)] & ~m_colorBB[m_turn];
        while (attacksBB != BITBOARD_ZERO) {
            moveBuffer.emplace_back(createMove(square, popLsb(attacksBB), 0));
        }
    }
    pieces = m_colorBB[m_turn] & m_pieceTypeBB[PieceTypes::ROOK];
    while (pieces != BITBOARD_ZERO) {
        Square   square = popLsb(pieces);
        Bitboard attacksBB =
            rookAttacks[square][getIndexOfOccupancy<PieceTypes::ROOK>(square, m_occupancy)] & ~m_colorBB[m_turn];
        while (attacksBB != BITBOARD_ZERO) {
            moveBuffer.emplace_back(createMove(square, popLsb(attacksBB), 0));
        }
    }

    pieces = m_colorBB[m_turn] & m_pieceTypeBB[PieceTypes::QUEEN];
    while (pieces != BITBOARD_ZERO) {
        Square   square    = popLsb(pieces);
        Bitboard attacksBB = (rookAttacks[square][getIndexOfOccupancy<PieceTypes::ROOK>(square, m_occupancy)] |
                              bishopAttacks[square][getIndexOfOccupancy<PieceTypes::BISHOP>(square, m_occupancy)]) &
                             ~m_colorBB[m_turn];
        while (attacksBB != BITBOARD_ZERO) {
            moveBuffer.emplace_back(createMove(square, popLsb(attacksBB), 0));
        }
    }

    pieces = m_colorBB[m_turn] & m_pieceTypeBB[PieceTypes::KING];
    while (pieces != 0ULL) {
        Square   square    = popLsb(pieces);
        Bitboard attacksBB = pseudoAttacks[PieceTypes::KING][square] & ~m_colorBB[m_turn];
        while (attacksBB != BITBOARD_ZERO) {
            moveBuffer.emplace_back(createMove(square, popLsb(attacksBB), 0));
        }
    }
    return moveBuffer.size() - prevBufferSize;
}

bool Position::isLegal() {
    Square king = lsb(m_colorBB[m_us] & m_pieceTypeBB[PieceTypes::KING]);

    Bitboard enemyAttackers = BITBOARD_ZERO;

    enemyAttackers |= pawnAttacks[m_us][king] & (m_colorBB[m_them] & m_pieceTypeBB[PieceTypes::PAWN]);
    enemyAttackers |= pseudoAttacks[PieceTypes::KING][king] & (m_colorBB[m_them] & m_pieceTypeBB[PieceTypes::KING]);
    enemyAttackers |= pseudoAttacks[PieceTypes::KNIGHT][king] & (m_colorBB[m_them] & m_pieceTypeBB[PieceTypes::KNIGHT]);

    enemyAttackers |= bishopAttacks[king][getIndexOfOccupancy<PieceTypes::BISHOP>(king, m_occupancy)] &
                      (m_colorBB[m_them] & (m_pieceTypeBB[PieceTypes::BISHOP] | m_pieceTypeBB[PieceTypes::QUEEN]));
    enemyAttackers |= rookAttacks[king][getIndexOfOccupancy<PieceTypes::ROOK>(king, m_occupancy)] &
                      (m_colorBB[m_them] & (m_pieceTypeBB[PieceTypes::ROOK] | m_pieceTypeBB[PieceTypes::QUEEN]));

    return enemyAttackers == BITBOARD_ZERO;
}

int Position::perft(uint8_t depth, bool verbose) {
    size_t startPos  = m_moveBuffer.size();
    size_t moveCount = generatePseudoLegalMoves(m_moveBuffer);

    int nodes = 0;

    if (depth == 1) {
        for (size_t i = 0; i < moveCount; ++i) {
            doMove(m_moveBuffer[startPos + i]);
            if (isLegal()) ++nodes;
            undoMove();
        }
        m_moveBuffer.resize(m_moveBuffer.size() - moveCount);
        return nodes;
    }

    for (size_t i = 0; i < moveCount; ++i) {
        const Move &move = m_moveBuffer[startPos + i];
        doMove(move);
        if (isLegal()) {
            int toAdd = perft(depth - 1);
            if (verbose) {
                std::cout << squareToString(getFrom(move)) << squareToString(getTo(move)) << ": " << toAdd << "\n";
            }
            nodes += toAdd;
        }
        undoMove();
    }
    m_moveBuffer.resize(startPos);
    return nodes;
}
