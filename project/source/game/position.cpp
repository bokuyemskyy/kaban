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
                int current_rank = File::NB - 1;
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
                    m_turn = Color::WHITE;
                else
                    m_turn = Color::BLACK;
                break;
            default:
                break;
        }
        ++argument_index;
    }
}

std::string Position::toFEN() const {
    std::stringstream fen;

    for (int r = Rank::NB - 1; r >= 0; r--) {
        int empty = 0;
        for (int f = 0; f < File::NB; ++f) {
            Square s = createSquare(File(f), Rank(r));
            Piece  p = m_board[s];

            if (p == Piece::NONE) {
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
    assert(square != Square::NONE);
    assert(piece != Piece::NONE);

    unsetPiece(square);
    m_colorBB[getColor(piece)] |= squareBB(square);
    m_pieceTypeBB[getPieceType(piece)] |= squareBB(square);
    m_board[square] = piece;
}

void Position::unsetPiece(Square square) {
    assert(square != Square::NONE);

    if (m_board[square] == Piece::NONE) return;
    m_colorBB[getColor(m_board[square])] &= ~squareBB(square);
    m_pieceTypeBB[getPieceType(m_board[square])] &= ~squareBB(square);
    m_board[square] = Piece::NONE;
}
void Position::resetBoard() {
    for (uint8_t i = 0; i < Color::NB; ++i) {
        m_colorBB[i] = Bitboard::zero();
    }
    for (uint8_t i = 0; i < PieceType::NB; ++i) {
        m_pieceTypeBB[i] = Bitboard::zero();
    }
    for (uint8_t i = 0; i < Square::NB; ++i) {
        m_board[i] = Piece::NONE;
    }
    m_turn = Color::WHITE;
}

inline void Position::toggleTurn() { m_turn = !m_turn; }

void Position::doMove(const Move move) {
    Square from = getFrom(move);
    Square to   = getTo(move);

    Bitboard moveBB = squareBB(from) | squareBB(to);

    m_deltas.emplace_back(createDelta(m_board[to], m_castling, 0, m_halfmoves));
    m_moves.emplace_back(move);

    if (m_board[to] != Piece::NONE) {
        Bitboard maskBB = ~squareBB(to);
        m_colorBB[getColor(m_board[to])] &= maskBB;
        m_pieceTypeBB[getPieceType(m_board[to])] &= maskBB;
    }

    m_colorBB[getColor(m_board[from])] ^= moveBB;
    m_pieceTypeBB[getPieceType(m_board[from])] ^= moveBB;

    m_board[to]   = m_board[from];
    m_board[from] = Piece::NONE;

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

    if (capturedPiece != Piece::NONE) {
        Bitboard toBB = squareBB(to);
        m_colorBB[getColor(capturedPiece)] |= toBB;
        m_pieceTypeBB[getPieceType(capturedPiece)] |= toBB;
    }

    m_castling  = getCastling(delta);
    m_halfmoves = getHalfmoves(delta);

    toggleTurn();

    updateCachedState();
}
