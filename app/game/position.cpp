#include "position.hpp"

#include <imgui.h>

#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

#include "bitboard.hpp"
#include "navigation.hpp"
#include "piece.hpp"

void Position::setFromFEN(const std::string &fen) {
    clear();

    std::stringstream ss(fen);
    std::string       argument;
    size_t            argument_index = 0;
    while (std::getline(ss, argument, ' ')) {
        switch (argument_index) {
            case 0: {
                int current_rank = File::number() - 1;
                int current_file = 0;

                for (char c : argument) {
                    if (c == '/') {
                        current_rank--;
                        current_file = 0;
                    } else if (std::isdigit(c) != 0) {
                        int num = c - '0';
                        current_file += num;
                    } else {
                        Square s = Square(File(current_file), Rank(current_rank));
                        set(s, Piece(c));
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

    for (int r = Rank::number() - 1; r >= 0; r--) {
        int empty = 0;
        for (int f = 0; f < File::number(); ++f) {
            Square s = Square(File(f), Rank(r));
            Piece  p = m_board[s.value()];

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

void Position::set(Square square, Piece piece) {
    unset(square);
    m_color_bb[piece.color()] |= Bitboard::square(square);
    m_piece_type_bb[piece.pieceType()] |= Bitboard::square(square);
    m_board[square.value()] = piece;
}

void Position::unset(Square square) {
    if (m_board[square.value()] == Piece::NONE) return;
    m_color_bb[m_board[square.value()].color()] &= ~Bitboard::square(square);
    m_piece_type_bb[m_board[square.value()].pieceType()] &= ~Bitboard::square(square);
    m_board[square.value()] = Piece::NONE;
}
void Position::clear() {
    for (Square square : Squares::all()) {
        unset(square);
    }
    m_turn = Color::WHITE;
}

inline void Position::nextTurn() { m_turn = !m_turn; }

void Position::doMove(const Move move) {
    Square from = move.from();
    Square to   = move.to();

    Bitboard moveBB = Bitboard::square(from) | Bitboard::square(to);

    // m_deltas.emplace_back(Delta(m_board[to], m_castling, 0, m_halfmoves));
    m_moves.emplace_back(move);

    if (m_board[to.value()] != Piece::NONE) {
        Bitboard maskBB = ~Bitboard::square(to);
        m_color_bb[m_board[to.value()].color()] &= maskBB;
        m_piece_type_bb[m_board[to.value()].pieceType()] &= maskBB;
    }

    m_color_bb[m_board[from.value()].color()] ^= moveBB;
    m_piece_type_bb[m_board[from.value()].pieceType()] ^= moveBB;

    m_board[to.value()]   = m_board[from.value()];
    m_board[from.value()] = Piece::NONE;

    nextTurn();
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

    Bitboard moveBB = Bitboard::square(from) | Bitboard::square(to);

    Piece movedPiece = m_board[to.value()];
    // Piece capturedPiece = getCaptured(delta);

    m_board[from.value()] = movedPiece;
    // m_board[to]   = capturedPiece;

    m_color_bb[movedPiece.color()] ^= moveBB;
    m_piece_type_bb[movedPiece.pieceType()] ^= moveBB;

    /*if (capturedPiece != Piece::NONE) {
        Bitboard toBB = squareBB(to);
        m_color_bb[getColor(capturedPiece)] |= toBB;
        m_piece_type_bb[getPieceType(capturedPiece)] |= toBB;
    }*/

    // m_castling  = getCastling(delta);
    // m_halfmoves = getHalfmoves(delta);

    nextTurn();
}
