#include "position.hpp"

#include <imgui.h>

#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

#include "bitboard.hpp"
#include "color.hpp"
#include "direction.hpp"
#include "en_passant.hpp"
#include "file.hpp"
#include "move_flag.hpp"
#include "piece.hpp"
#include "piece_type.hpp"
#include "rank.hpp"
#include "square.hpp"
#include "undo_info.hpp"

void Position::fromFen(const std::string& fen) {
    m_stm = Colors::WHITE;
    for (Square square : Squares::all()) {
        if (at(square).hasValue()) unsetPiece(square);
    }

    std::stringstream ss(fen);
    std::string       argument;
    size_t            argument_index = 0;

    while (std::getline(ss, argument, ' ')) {
        switch (argument_index) {
            case 0: {
                int current_rank = Files::count() - 1;
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
                        setPiece(s, p);
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

    for (int r = Ranks::count() - 1; r >= 0; r--) {
        int empty = 0;
        for (int f = 0; f < Files::count(); ++f) {
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

void Position::setPiece(Square square, Piece piece) {
    auto mask = Bitboard::square(square);
    at(piece.color()) |= mask;
    at(piece.type()) |= mask;
    at(square) = piece;
}

void Position::unsetPiece(Square square) {
    Piece piece = at(square);
    auto  mask  = Bitboard::square(square);
    m_color[piece.color().value()] &= ~mask;
    m_piece_type[piece.type().value()] &= ~mask;
    m_board[square.value()] = Pieces::NONE;
}

void Position::movePiece(Square from, Square to) {
    Piece    piece     = at(from);
    Bitboard move_mask = Bitboard::square(from) | Bitboard::square(to);
    m_color[piece.color().value()] ^= move_mask;
    m_piece_type[piece.type().value()] ^= move_mask;
    m_board[from.value()] = Pieces::NONE;
    m_board[to.value()]   = piece;
}

UndoInfo Position::makeMove(const Move move) {
    UndoInfo undo_info = {m_castling, m_en_passant, m_halfmove};

    Square from = move.from();
    Square to   = move.to();

    Piece captured = move.flag() == MoveFlags::EN_PASSANT ? Piece(!m_stm, PieceTypes::PAWN) : at(to);

    if (captured.hasValue()) {
        Square captured_square = to;

        if (captured.type() == PieceTypes::PAWN) {
            if (move.flag() == MoveFlags::EN_PASSANT) {
                captured_square = captured_square + (m_stm == Colors::WHITE ? Directions::S : Directions::N);
            }
        }

        unsetPiece(captured_square);
    }

    movePiece(from, to);

    if (move.flag() == MoveFlags::PAWN_DOUBLE_PUSH) {
        m_en_passant.set(to.file());
    } else {
        m_en_passant.clear();
    }

    m_stm.flip();

    undo_info.setCaptured(captured);

    return undo_info;
}
void Position::unmakeMove(const Move move, const UndoInfo& undo_info) {
    m_stm.flip();

    Square from = move.from();
    Square to   = move.to();

    movePiece(to, from);

    if (undo_info.captured().hasValue()) {
        Square captured_square = to;
        if (undo_info.captured().type() == PieceTypes::PAWN && move.flag() == MoveFlags::EN_PASSANT) {
            captured_square = captured_square + (m_stm == Colors::WHITE ? Directions::S : Directions::N);
        }
        setPiece(captured_square, undo_info.captured());
    }

    m_en_passant = undo_info.enPassant();
    m_castling   = undo_info.castling();
    m_halfmove   = undo_info.halfmove();
}