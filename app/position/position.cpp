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
#include "undo_info.hpp"

void Position::fromFen(const std::string &fen) {
    for (Square square : Squares::all()) {
        unset(square);
    }
    m_stm = Colors::WHITE;

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
                        Piece  p = Piece::fromChar(c);
                        set(s, p);
                        ++current_file;
                    }
                }
                break;
            }
            case 1:
                if (argument == "b")
                    m_stm = Colors::BLACK;
                else
                    m_stm = Colors::WHITE;
                break;
            default:
                break;
        }
        ++argument_index;
    }
}

std::string Position::toFen() const {
    std::stringstream fen;

    for (int r = Rank::count() - 1; r >= 0; r--) {
        int empty = 0;
        for (int f = 0; f < File::count(); ++f) {
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
    m_color[piece.color().value()] |= Bitboard::square(square);
    m_pieceType[piece.pieceType().value()] |= Bitboard::square(square);
    m_board[square.value()] = piece;
}

void Position::unset(Square square) {
    Piece set_piece = at(square);
    if (!at(square).hasValue()) return;
    m_color[set_piece.color().value()] &= ~Bitboard::square(square);
    m_pieceType[set_piece.pieceType().value()] &= ~Bitboard::square(square);
    m_board[square.value()] = Pieces::NONE;
}

UndoInfo Position::makeMove(const Move move) {
    UndoInfo undo_info = {m_castling, m_en_passant, m_halfmove};

    Square from = move.from();
    Square to   = move.to();

    Piece moved    = at(from);
    Piece captured = at(to);

    if (captured.hasValue()) {
        Bitboard to_mask = Bitboard::square(to);
        m_color[captured.color().value()] &= ~to_mask;
        m_pieceType[captured.pieceType().value()] &= ~to_mask;
    }

    Bitboard move_mask    = Bitboard::square(from) | Bitboard::square(to);
    m_board[to.value()]   = moved;
    m_board[from.value()] = Pieces::NONE;
    m_color[moved.color().value()] ^= move_mask;
    m_pieceType[moved.pieceType().value()] ^= move_mask;

    m_stm.flip();

    undo_info.setCaptured(captured);
    return undo_info;
}

void Position::unmakeMove(Move move, UndoInfo undo_info) {
    Square from = move.from();
    Square to   = move.to();

    Piece moved    = at(to);
    Piece captured = undo_info.captured();

    Bitboard move_mask    = Bitboard::square(from) | Bitboard::square(to);
    m_board[to.value()]   = captured;
    m_board[from.value()] = moved;
    m_color[moved.color().value()] ^= move_mask;
    m_pieceType[moved.pieceType().value()] ^= move_mask;

    if (captured.hasValue()) {
        Bitboard to_mask = Bitboard::square(to);
        m_color[captured.color().value()] |= to_mask;
        m_pieceType[captured.pieceType().value()] |= to_mask;
    }

    m_castling   = undo_info.castling();
    m_en_passant = undo_info.enPassant();
    m_halfmove   = undo_info.halfmove();

    m_stm.flip();
}
