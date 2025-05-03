#include "position.hpp"

#include <imgui.h>

#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

#include "bitboard.hpp"
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
    m_colorBB[Colors::WHITE] = BITBOARD_ZERO;
    m_colorBB[Colors::BLACK] = BITBOARD_ZERO;
    for (uint8_t i = 0; i < PieceTypes::NB; ++i) {
        m_pieceTypeBB[i] = BITBOARD_ZERO;
    }
    for (uint8_t i = 0; i < Squares::NB; ++i) {
        m_board[i] = Pieces::NONE;
    }
    m_turn = Colors::WHITE;
}

void Position::doMove(const Move move) {
    m_deltas.emplace_back(createDelta(m_board[getTo(move)], m_castling, 0, m_halfmoves));
    m_moves.emplace_back(move);

    setPiece(getTo(move), m_board[getFrom(move)]);
    unsetPiece(getFrom(move));

    m_turn = !m_turn;
}

void Position::undoMove() {
    if (m_deltas.empty() || m_moves.empty()) return;

    Delta delta = m_deltas.back();
    Move  move  = m_moves.back();
    m_deltas.pop_back();
    m_moves.pop_back();

    Square  from  = getFrom(move);
    Square  to    = getTo(move);
    uint8_t flags = getFlags(move);

    setPiece(from, m_board[to]);
    if (getCaptured(delta) != Pieces::NONE)
        setPiece(to, getCaptured(delta));
    else
        unsetPiece(to);

    m_castling  = getCastling(delta);
    m_halfmoves = getHalfmoves(delta);

    m_turn = !m_turn;
}

void Position::generatePawnMoves(std::vector<Move> &moves, Square square) const {
    Direction              moveDirection    = (m_turn == Colors::WHITE) ? Directions::N : Directions::S;
    std::vector<Direction> attackDirections = (m_turn == Colors::WHITE)
                                                  ? std::vector<Direction>{Directions::NE, Directions::NW}
                                                  : std::vector<Direction>{Directions::SE, Directions::SW};

    Bitboard singlePush = destinationBB(square, moveDirection) & ~m_colorBB[Colors::WHITE] & ~m_colorBB[Colors::BLACK];

    if (singlePush != BITBOARD_ZERO) {
        if (isPawnStartingRank(getRank(square), m_turn)) {
            Bitboard doublePush =
                destinationBB(square, moveDirection * 2) & ~m_colorBB[Colors::WHITE] & ~m_colorBB[Colors::BLACK];

            if (doublePush != BITBOARD_ZERO) {
                moves.emplace_back(createMove(square, popLsb(doublePush), 0));
            }
        }
        moves.emplace_back(createMove(square, popLsb(singlePush), 0));
    }

    for (const auto &direction : attackDirections) {
        Bitboard attack = destinationBB(square, direction) & m_colorBB[!m_turn];
        if (attack != BITBOARD_ZERO) {
            moves.emplace_back(createMove(square, popLsb(attack), 0));
        }
    }
}
void Position::generateKnightMoves(std::vector<Move> &moves, Square square) const {}
void Position::generateBishopMoves(std::vector<Move> &moves, Square square) const {
    for (const auto &direction : bishopDirections) {
        for (uint8_t i = 1;; ++i) {
            Bitboard destination = destinationBB(square + (direction * (i - 1)), direction);
            if (destination == BITBOARD_ZERO) break;

            if ((m_colorBB[m_turn] & destination) != BITBOARD_ZERO) break;
            moves.emplace_back(createMove(square, lsb(destination), 0));
            if ((m_colorBB[!m_turn] & destination) != BITBOARD_ZERO) break;
        }
    }
}

void Position::generateRookMoves(std::vector<Move> &moves, Square square) const {
    for (const auto &direction : rookDirections) {
        Bitboard destination = destinationBB(square, direction);
        for (uint8_t i = 1;; ++i) {
            Bitboard destination = destinationBB(square + (direction * (i - 1)), direction);
            if (destination == BITBOARD_ZERO) break;

            if (m_colorBB[m_turn] & destination) break;
            moves.emplace_back(createMove(square, lsb(destination), 0));
            if (m_colorBB[!m_turn] & destination) break;
        }
    }
}

void Position::generateQueenMoves(std::vector<Move> &moves, Square square) const {
    for (const auto &direction : queenDirections) {
        for (uint8_t i = 1;; ++i) {
            Bitboard destination = destinationBB(square + (direction * (i - 1)), direction);
            if (destination == BITBOARD_ZERO) break;

            if (m_colorBB[m_turn] & destination) break;
            moves.emplace_back(createMove(square, lsb(destination), 0));
            if (m_colorBB[!m_turn] & destination) break;
        }
    }
}

void Position::generateKingMoves(std::vector<Move> &moves, Square square) const {}

void Position::generatePseudoLegalMoves(std::vector<Move> &moves) {
    Bitboard pawns = m_colorBB[m_turn] & m_pieceTypeBB[PieceTypes::PAWN];
    while (pawns != 0ULL) {
        Square s = popLsb(pawns);
        generatePawnMoves(moves, s);
    }
    Bitboard knights = m_colorBB[m_turn] & m_pieceTypeBB[PieceTypes::KNIGHT];
    while (knights != BITBOARD_ZERO) {
        Square   square    = popLsb(knights);
        Bitboard attacksBB = pseudoAttacks[PieceTypes::KNIGHT][square] & ~m_colorBB[m_turn];
        while (attacksBB != BITBOARD_ZERO) {
            moves.emplace_back(createMove(square, popLsb(attacksBB), 0));
        }
    }
    Bitboard bishops = m_colorBB[m_turn] & m_pieceTypeBB[PieceTypes::BISHOP];
    while (bishops != 0ULL) {
        Square s = popLsb(bishops);
        generateBishopMoves(moves, s);
    }
    Bitboard rooks = m_colorBB[m_turn] & m_pieceTypeBB[PieceTypes::ROOK];
    while (rooks != 0ULL) {
        Square   square = popLsb(rooks);
        Bitboard attacksBB =
            rookAttacks[square][magicMap(square, m_colorBB[Colors::WHITE] | m_colorBB[Colors::BLACK])] &
            ~m_colorBB[m_turn];
        while (attacksBB != BITBOARD_ZERO) {
            moves.emplace_back(createMove(square, popLsb(attacksBB), 0));
        }
    }
    Bitboard queens = m_colorBB[m_turn] & m_pieceTypeBB[PieceTypes::QUEEN];
    while (queens != 0ULL) {
        Square s = popLsb(queens);
        generateQueenMoves(moves, s);
    }
    Bitboard kings = m_colorBB[m_turn] & m_pieceTypeBB[PieceTypes::KING];
    while (kings != 0ULL) {
        Square   square    = popLsb(kings);
        Bitboard attacksBB = pseudoAttacks[PieceTypes::KING][square] & ~m_colorBB[m_turn];
        while (attacksBB != BITBOARD_ZERO) {
            moves.emplace_back(createMove(square, popLsb(attacksBB), 0));
        }
    }
}

bool Position::isLegal() {
    Color us   = !m_turn;
    Color them = m_turn;

    Square king   = lsb(m_colorBB[us] & m_pieceTypeBB[PieceTypes::KING]);
    Square opKing = lsb(m_colorBB[them] & m_pieceTypeBB[PieceTypes::KING]);

    Direction pawnLeft  = (us == Colors::WHITE) ? Directions::NW : Directions::SW;
    Direction pawnRight = (us == Colors::WHITE) ? Directions::NE : Directions::SE;

    if (m_board[king + pawnLeft] == createPiece(them, PieceTypes::PAWN) ||
        m_board[king + pawnRight] == createPiece(them, PieceTypes::PAWN))
        return false;

    if (abs(static_cast<int>(getRank(king)) - static_cast<int>(getRank(opKing))) <= 1 &&
        abs(static_cast<int>(getFile(king)) - static_cast<int>(getFile(opKing))) <= 1)
        return false;

    for (const auto &dir : knightDirections) {
        Bitboard destination = destinationBB(king, dir);
        if (destination != BITBOARD_ZERO &&
            (m_colorBB[them] & m_pieceTypeBB[PieceTypes::KNIGHT] & destination) != BITBOARD_ZERO)
            return false;
    }

    for (const auto &dir : bishopDirections) {
        Square current = king;
        while (true) {
            Bitboard destBB = destinationBB(current, dir);
            if (destBB == BITBOARD_ZERO) break;

            Square dst = lsb(destBB);

            Piece p = m_board[dst];
            if (p == createPiece(them, PieceTypes::QUEEN) || p == createPiece(them, PieceTypes::BISHOP)) return false;
            if (p != Pieces::NONE) break;

            current = dst;
        }
    }

    for (const auto &dir : rookDirections) {
        Square current = king;
        while (true) {
            Bitboard destBB = destinationBB(current, dir);
            if (destBB == BITBOARD_ZERO) break;

            Square dst = lsb(destBB);

            Piece p = m_board[dst];
            if (p == createPiece(them, PieceTypes::QUEEN) || p == createPiece(them, PieceTypes::ROOK)) return false;
            if (p != Pieces::NONE) break;

            current = dst;
        }
    }

    return true;
}

int Position::perft(uint8_t depth, bool verbose) {
    std::vector<Move> moves;
    moves.reserve(MAX_MOVES);

    int nodes = 0;

    if (depth == 0) return 1ULL;

    generatePseudoLegalMoves(moves);

    for (const auto &move : moves) {
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
    return nodes;
}
