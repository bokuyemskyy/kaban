#include "position.hpp"

#include <imgui.h>

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

#include "bitboard.hpp"
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
        m_colorBB[i] = Bitboards::ZERO;
    }
    for (uint8_t i = 0; i < PieceTypes::NB; ++i) {
        m_pieceTypeBB[i] = Bitboards::ZERO;
    }
    for (uint8_t i = 0; i < Squares::NB; ++i) {
        m_board[i] = Pieces::NONE;
    }
    m_turn = Colors::WHITE;
}

inline void Position::toggleTurn() { m_turn = !m_turn; }

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
