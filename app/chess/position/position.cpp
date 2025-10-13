#include "position.hpp"


#include <cassert>
#include <deque>
#include <iostream>
#include <sstream>
#include <string>

#include "bitboard.hpp"
#include "castling.hpp"
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

void Position::reset() {
    m_stm      = Colors::WHITE;
    m_castling = Castlings::ANY;

    m_en_passant.clear();
    m_halfmove.reset();

    for (Square square : Squares::all()) {
        if (at(square).hasValue()) unsetPiece(square);
    }
}
void Position::fromFen(const std::string& fen) {
    reset();

    std::istringstream      iss(fen);
    std::deque<std::string> tokens;
    std::string             token;
    while (iss >> token) tokens.push_back(token);
    if (tokens.size() < 2) {
        std::cerr << "Invalid FEN. At least 2 fields are required." << std::endl;
        return;
    }

    std::string piece_placement = tokens.front();
    tokens.pop_front();

    int current_rank = Files::count() - 1;
    int current_file = 0;
    for (char c : piece_placement) {
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

    std::string side_to_move = tokens.front();
    tokens.pop_front();

    switch (side_to_move[0]) {
        case 'w':
            m_stm = Colors::WHITE;
            break;
        case 'b':
            m_stm = Colors::BLACK;
            break;
        default:
            std::cerr << "Invalid side to move. Use 'w' or 'b'." << std::endl;
            return;
    }

    if (tokens.empty()) return;
    std::string castling = tokens.front();
    tokens.pop_front();

    if (castling[0] != '-') {
        m_castling = Castlings::NONE;
        for (char c : castling) {
            switch (c) {
                case 'K':
                    m_castling.add(Castlings::W_KING_SIDE);
                    break;
                case 'k':
                    m_castling.add(Castlings::B_KING_SIDE);
                    break;
                case 'Q':
                    m_castling.add(Castlings::W_QUEEN_SIDE);
                    break;
                case 'q':
                    m_castling.add(Castlings::B_QUEEN_SIDE);
                    break;
            }
        }
    } else {
        m_castling = Castlings::NONE;
    }

    if (tokens.empty()) return;
    std::string en_passant = tokens.front();
    tokens.pop_front();

    if (en_passant[0] != '-') {
        m_en_passant.set(Square::fromString(en_passant).file());
    }

    if (tokens.empty()) return;
    std::string halfmove = tokens.front();
    tokens.pop_front();

    m_halfmove.set(std::stoi(halfmove));
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

    if (move.flag() == MoveFlags::PROMOTION_QUEEN || move.flag() == MoveFlags::PROMOTION_ROOK ||
        move.flag() == MoveFlags::PROMOTION_BISHOP || move.flag() == MoveFlags::PROMOTION_KNIGHT) {
        PieceType promo_type;
        if (move.flag() == MoveFlags::PROMOTION_QUEEN)
            promo_type = PieceTypes::QUEEN;
        else if (move.flag() == MoveFlags::PROMOTION_ROOK)
            promo_type = PieceTypes::ROOK;
        else if (move.flag() == MoveFlags::PROMOTION_BISHOP)
            promo_type = PieceTypes::BISHOP;
        else
            promo_type = PieceTypes::KNIGHT;

        unsetPiece(to);
        setPiece(to, Piece(m_stm, promo_type));
    }

    if (move.flag() == MoveFlags::CASTLING_KING) {
        auto rank = (m_stm == Colors::WHITE ? Ranks::R1 : Ranks::R8);
        movePiece(Square(Files::FH, rank), Square(Files::FF, rank));
    } else if (move.flag() == MoveFlags::CASTLING_QUEEN) {
        auto rank = (m_stm == Colors::WHITE ? Ranks::R1 : Ranks::R8);
        movePiece(Square(Files::FA, rank), Square(Files::FD, rank));
    }

    if (from == Squares::A1 || to == Squares::A1) m_castling.remove(Castlings::W_QUEEN_SIDE);
    if (from == Squares::E1 || to == Squares::E1) m_castling.remove(Castlings::W_SIDE);
    if (from == Squares::H1 || to == Squares::H1) m_castling.remove(Castlings::W_KING_SIDE);
    if (from == Squares::A8 || to == Squares::A8) m_castling.remove(Castlings::B_QUEEN_SIDE);
    if (from == Squares::E8 || to == Squares::E8) m_castling.remove(Castlings::B_SIDE);
    if (from == Squares::H8 || to == Squares::H8) m_castling.remove(Castlings::B_KING_SIDE);

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

    if (move.flag() == MoveFlags::CASTLING_KING) {
        auto rank = (m_stm == Colors::WHITE ? Ranks::R1 : Ranks::R8);
        movePiece(Square(Files::FF, rank), Square(Files::FH, rank));
    } else if (move.flag() == MoveFlags::CASTLING_QUEEN) {
        auto rank = (m_stm == Colors::WHITE ? Ranks::R1 : Ranks::R8);
        movePiece(Square(Files::FD, rank), Square(Files::FA, rank));
    }

    if (move.flag() == MoveFlags::PROMOTION_QUEEN || move.flag() == MoveFlags::PROMOTION_ROOK ||
        move.flag() == MoveFlags::PROMOTION_BISHOP || move.flag() == MoveFlags::PROMOTION_KNIGHT) {
        unsetPiece(to);
        setPiece(from, Piece(m_stm, PieceTypes::PAWN));
    } else {
        movePiece(to, from);
    }

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