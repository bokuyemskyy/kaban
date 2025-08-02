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
                        Square s = Square(current_file, current_rank);
                        setPiece(s, Piece(c));
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
            Square s = Square(File(f), Rank(r));
            Piece  p = m_board[s];

            if (p == Piece::NONE) {
                ++empty;
            } else {
                if (empty > 0) {
                    fen << empty;
                    empty = 0;
                }
                fen << p.to_char();
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
    m_colorBB[piece.color()] |= Bitboard::squareBB(square);
    m_pieceTypeBB[piece.pieceType()] |= Bitboard::squareBB(square);
    m_board[square] = piece;
}

void Position::unsetPiece(Square square) {
    assert(square != Square::NONE);

    if (m_board[square] == Piece::NONE) return;
    m_colorBB[m_board[square].color()] &= ~Bitboard::squareBB(square);
    m_pieceTypeBB[m_board[square].pieceType()] &= ~Bitboard::squareBB(square);
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
    Square from = move.from();
    Square to   = move.to();

    Bitboard moveBB = Bitboard::squareBB(from) | Bitboard::squareBB(to);

    // m_deltas.emplace_back(Delta(m_board[to], m_castling, 0, m_halfmoves));
    m_moves.emplace_back(move);

    if (m_board[to] != Piece::NONE) {
        Bitboard maskBB = ~Bitboard::squareBB(to);
        m_colorBB[m_board[to].color()] &= maskBB;
        m_pieceTypeBB[m_board[to].pieceType()] &= maskBB;
    }

    m_colorBB[m_board[from].color()] ^= moveBB;
    m_pieceTypeBB[m_board[from].pieceType()] ^= moveBB;

    m_board[to]   = m_board[from];
    m_board[from] = Piece::NONE;

    toggleTurn();
}

void Position::undoMove() {
    // DELTA FUNCTIONALITY WILL BE REPLACED BY GAMESTATE

    if (/*m_deltas.empty() ||*/ m_moves.empty()) return;

    // Delta delta = m_deltas.back();
    Move move = m_moves.back();

    // m_deltas.pop_back();
    m_moves.pop_back();

    Square from = move.from();
    Square to   = move.to();

    Bitboard moveBB = Bitboard::squareBB(from) | Bitboard::squareBB(to);

    Piece movedPiece = m_board[to];
    // Piece capturedPiece = getCaptured(delta);

    m_board[from] = movedPiece;
    // m_board[to]   = capturedPiece;

    m_colorBB[movedPiece.color()] ^= moveBB;
    m_pieceTypeBB[movedPiece.pieceType()] ^= moveBB;

    /*if (capturedPiece != Piece::NONE) {
        Bitboard toBB = squareBB(to);
        m_colorBB[getColor(capturedPiece)] |= toBB;
        m_pieceTypeBB[getPieceType(capturedPiece)] |= toBB;
    }*/

    // m_castling  = getCastling(delta);
    // m_halfmoves = getHalfmoves(delta);

    toggleTurn();
}
