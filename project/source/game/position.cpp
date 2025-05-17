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

    m_turn = !m_turn;
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

void Position::generatePseudoLegalMoves(std::vector<Move> &moves) {
    Bitboard pawns = m_colorBB[m_turn] & m_pieceTypeBB[PieceTypes::PAWN];
    while (pawns != BITBOARD_ZERO) {
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
    while (bishops != BITBOARD_ZERO) {
        Square   square    = popLsb(bishops);
        Bitboard attacksBB = bishopAttacks[square][getIndexOfOccupancy<PieceTypes::BISHOP>(
                                 square, m_colorBB[Colors::WHITE] | m_colorBB[Colors::BLACK])] &
                             ~m_colorBB[m_turn];
        while (attacksBB != BITBOARD_ZERO) {
            moves.emplace_back(createMove(square, popLsb(attacksBB), 0));
        }
    }
    Bitboard rooks = m_colorBB[m_turn] & m_pieceTypeBB[PieceTypes::ROOK];
    while (rooks != BITBOARD_ZERO) {
        Square   square    = popLsb(rooks);
        Bitboard attacksBB = rookAttacks[square][getIndexOfOccupancy<PieceTypes::ROOK>(
                                 square, m_colorBB[Colors::WHITE] | m_colorBB[Colors::BLACK])] &
                             ~m_colorBB[m_turn];
        while (attacksBB != BITBOARD_ZERO) {
            moves.emplace_back(createMove(square, popLsb(attacksBB), 0));
        }
    }

    Bitboard queens = m_colorBB[m_turn] & m_pieceTypeBB[PieceTypes::QUEEN];
    while (queens != BITBOARD_ZERO) {
        Square   square    = popLsb(queens);
        Bitboard attacksBB = (rookAttacks[square][getIndexOfOccupancy<PieceTypes::ROOK>(
                                  square, m_colorBB[Colors::WHITE] | m_colorBB[Colors::BLACK])] |
                              bishopAttacks[square][getIndexOfOccupancy<PieceTypes::BISHOP>(
                                  square, m_colorBB[Colors::WHITE] | m_colorBB[Colors::BLACK])]) &
                             ~m_colorBB[m_turn];
        while (attacksBB != BITBOARD_ZERO) {
            moves.emplace_back(createMove(square, popLsb(attacksBB), 0));
        }
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

    Square king = lsb(m_colorBB[us] & m_pieceTypeBB[PieceTypes::KING]);

    Bitboard pawnThreats = pawnAttacks[us][king] & (m_colorBB[them] & m_pieceTypeBB[PieceTypes::PAWN]);
    if (pawnThreats != BITBOARD_ZERO) return false;

    Bitboard kingThreats = pseudoAttacks[PieceTypes::KING][king] & (m_colorBB[them] & m_pieceTypeBB[PieceTypes::KING]);
    if (kingThreats != BITBOARD_ZERO) return false;

    Bitboard knightThreats =
        pseudoAttacks[PieceTypes::KNIGHT][king] & (m_colorBB[them] & m_pieceTypeBB[PieceTypes::KNIGHT]);
    if (knightThreats != BITBOARD_ZERO) return false;

    Bitboard diagThreats = bishopAttacks[king][getIndexOfOccupancy<PieceTypes::BISHOP>(
                               king, m_colorBB[Colors::WHITE] | m_colorBB[Colors::BLACK])] &
                           (m_colorBB[them] & (m_pieceTypeBB[PieceTypes::BISHOP] | m_pieceTypeBB[PieceTypes::QUEEN]));

    if (diagThreats != BITBOARD_ZERO) return false;

    Bitboard orthoThreats = rookAttacks[king][getIndexOfOccupancy<PieceTypes::ROOK>(
                                king, m_colorBB[Colors::WHITE] | m_colorBB[Colors::BLACK])] &
                            (m_colorBB[them] & (m_pieceTypeBB[PieceTypes::ROOK] | m_pieceTypeBB[PieceTypes::QUEEN]));
    return orthoThreats == BITBOARD_ZERO;
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
