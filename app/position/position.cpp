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

void Position::set_from_fen(const std::string &fen) {
    for (Square square : Squares::all()) {
        unset(square);
    }
    m_sideToMove = Colors::WHITE;

    std::stringstream ss(fen);
    std::string       argument;
    size_t            argument_index = 0;
    while (std::getline(ss, argument, ' ')) {
        switch (argument_index) {
            case 0: {
                int current_rank = File::count() - 1;
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
                        Piece  p = Piece::from_char(c);
                        set(s, p);
                        ++current_file;
                    }
                }
                break;
            }
            case 1:
                if (argument == "b")
                    m_sideToMove = Colors::BLACK;
                else
                    m_sideToMove = Colors::WHITE;
                break;
            default:
                break;
        }
        ++argument_index;
    }
}

std::string Position::to_fen() const {
    std::stringstream fen;

    for (int r = Rank::count() - 1; r >= 0; r--) {
        int empty = 0;
        for (int f = 0; f < File::count(); ++f) {
            Square s = Square(File(f), Rank(r));
            Piece  p = m_board[s.value()];

            if (p.has_value()) {
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
    m_color[piece.color().value()] |= Bitboard::square(square);
    m_pieceType[piece.piece_type().value()] |= Bitboard::square(square);
    m_board[square.value()] = piece;
}

void Position::unset(Square square) {
    Piece set_piece = at(square);
    if (!at(square).has_value()) return;
    m_color[set_piece.color().value()] &= ~Bitboard::square(square);
    m_pieceType[set_piece.piece_type().value()] &= ~Bitboard::square(square);
    m_board[square.value()] = Pieces::NONE;
}

void Position::do_move(const Move move) {
    Square from = move.from();
    Square to   = move.to();

    Bitboard moveBitboard = Bitboard::square(from) | Bitboard::square(to);

    // m_deltas.emplace_back(Delta(m_board[to], m_castling, 0, m_halfmoves));
    // m_moves.emplace_back(move);

    /*if (m_board[to.value()] != Piece::NONE) {
        Bitboard maskBB = ~Bitboard::square(to);
        m_color[m_board[to.value()].color()] &= maskBB;
        m_pieceType[m_board[to.value()].piece_type()] &= maskBB;
    }

    m_color[m_board[from.value()].color()] ^= moveBB;
    m_pieceType[m_board[from.value()].piece_type()] ^= moveBB;
*/
    m_board[to.value()] = m_board[from.value()];
    // m_board[from.value()] = Piece::NONE;

    m_sideToMove.flip();
}

void Position::undo_move(Move move, UndoInfo undoInfo) {
    // if (m_deltas.empty() || m_moves.empty()) return;

    // Delta delta = m_deltas.back();
    // Move move = m_moves.back();

    // m_deltas.pop_back();
    // m_moves.pop_back();

    // Square from = move.from();
    // Square to   = move.to();

    // Bitboard moveBB = Bitboard::square(from) | Bitboard::square(to);

    // Piece movedPiece = m_board[to.value()];
    //  Piece capturedPiece = getCaptured(delta);

    // m_board[from.value()] = movedPiece;
    //  m_board[to]   = capturedPiece;

    // m_color[movedPiece.color().value()] ^= moveBB;
    // m_pieceType[movedPiece.piece_type().value()] ^= moveBB;

    /*if (capturedPiece != Piece::NONE) {
        Bitboard toBB = squareBB(to);
        m_color[getColor(capturedPiece)] |= toBB;
        m_pieceType[getPieceType(capturedPiece)] |= toBB;
    }*/

    // m_castling  = getCastling(delta);
    // m_halfmoves = getHalfmoves(delta);

    // m_sideToMove = !m_sideToMove;
}
