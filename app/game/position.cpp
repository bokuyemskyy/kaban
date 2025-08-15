#include "position.hpp"

#include <imgui.h>

#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

#include "bitboard.hpp"
#include "file.hpp"
#include "piece.hpp"
#include "rank.hpp"
#include "square.hpp"

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
                        Piece  p = Piece::fromChar(c);
                        set(s, p);
                        ++current_file;
                    }
                }
                break;
            }
            case 1:
                if (argument == "b")
                    m_turn = Colors::BLACK;
                else
                    m_turn = Colors::WHITE;
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

            if (p.hasValue()) {
                ++empty;
            } else {
                if (empty > 0) {
                    fen << empty;
                    empty = 0;
                }
                fen << p.toChar();
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
    m_color_bb[piece.color().value()] |= Bitboard::square(square);
    m_piece_type_bb[piece.pieceType().value()] |= Bitboard::square(square);
    m_board[square.value()] = piece;
}

void Position::unset(Square square) {
    Piece set_piece = at(square);
    if (!at(square).hasValue()) return;
    m_color_bb[set_piece.color().value()] &= ~Bitboard::square(square);
    m_piece_type_bb[set_piece.pieceType().value()] &= ~Bitboard::square(square);
    m_board[square.value()] = Pieces::NONE;
}
void Position::clear() {
    for (Square square : Squares::all()) {
        unset(square);
    }
    m_turn = Colors::WHITE;
}

inline void Position::nextTurn() { m_turn.flip(); }

void Position::doMove(const Move move) {
    Square from = move.from();
    Square to   = move.to();

    Bitboard moveBB = Bitboard::square(from) | Bitboard::square(to);

    // m_deltas.emplace_back(Delta(m_board[to], m_castling, 0, m_halfmoves));
    m_moves.emplace_back(move);

    /*if (m_board[to.value()] != Piece::NONE) {
        Bitboard maskBB = ~Bitboard::square(to);
        m_color_bb[m_board[to.value()].color()] &= maskBB;
        m_piece_type_bb[m_board[to.value()].pieceType()] &= maskBB;
    }

    m_color_bb[m_board[from.value()].color()] ^= moveBB;
    m_piece_type_bb[m_board[from.value()].pieceType()] ^= moveBB;
*/
    m_board[to.value()] = m_board[from.value()];
    // m_board[from.value()] = Piece::NONE;

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

    m_color_bb[movedPiece.color().value()] ^= moveBB;
    m_piece_type_bb[movedPiece.pieceType().value()] ^= moveBB;

    /*if (capturedPiece != Piece::NONE) {
        Bitboard toBB = squareBB(to);
        m_color_bb[getColor(capturedPiece)] |= toBB;
        m_piece_type_bb[getPieceType(capturedPiece)] |= toBB;
    }*/

    // m_castling  = getCastling(delta);
    // m_halfmoves = getHalfmoves(delta);

    nextTurn();
}
