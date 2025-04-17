#include "position.hpp"

#include <cassert>
#include <cstdint>
#include <sstream>
#include <string>
#include <vector>

#include "types.hpp"

Bitboard lsb(Bitboard &bitboard) {
    Bitboard lsb = bitboard & -bitboard;
    bitboard &= bitboard - 1;
    return lsb;
}

void Position::setFen(const std::string &fen) {
    std::stringstream ss(fen);
    std::string argument;
    size_t argument_index = 0;
    while (std::getline(ss, argument, ' ')) {
        switch (argument_index) {
            case 0: {
                int current_rank = BOARD_SIZE - 1;
                int current_file = 0;

                for (char c : argument) {
                    if (c == '/') {
                        current_rank--;
                        current_file = 0;
                    } else if (std::isdigit(c) != 0) {
                        int num = c - '0';
                        current_file += num;
                    } else {
                        Square s = makeSquare(static_cast<File>(current_file),
                                              static_cast<Rank>(current_rank));
                        setPiece(s, charToPiece(c));
                        current_file++;
                    }
                }
                break;
            }
            default:
                break;
        }
        argument_index++;
    }
}

std::string Position::getFen() const {
    std::stringstream fen;

    for (int r = BOARD_SIZE - 1; r >= 0; r--) {
        int empty = 0;
        for (int f = 0; f < BOARD_SIZE; f++) {
            Square s = makeSquare(File(f), Rank(r));
            Piece p = pieceAt(s);

            if (p == Piece::NONE) {
                empty++;
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

void Position::setPiece(Square s, Piece p) {
    assert(p != Piece::NONE);
    assert(s != Square::NONE);

    unsetPiece(s);
    m_bitboards.at(static_cast<uint8_t>(p)) |= (1ULL << static_cast<uint8_t>(s));
}

void Position::unsetPiece(Square s) {
    assert(s != Square::NONE);

    uint64_t mask = 1ULL << static_cast<uint8_t>(s);

    for (auto i = static_cast<uint8_t>(Piece::FIRST); i <= static_cast<uint8_t>(Piece::LAST); ++i) {
        if ((m_bitboards.at(i) & mask) != 0ULL) {
            m_bitboards.at(i) &= ~mask;
            break;
        }
    }
}

Piece Position::pieceAt(Square s) const {
    assert(s != Square::NONE);

    uint64_t mask = 1ULL << static_cast<uint8_t>(s);

    for (auto i = static_cast<uint8_t>(Piece::FIRST); i <= static_cast<uint8_t>(Piece::LAST); ++i) {
        if ((m_bitboards.at(i) & mask) != 0ULL) return Piece(i);
    }
    return Piece::NONE;
}

void Position::doMove(const Move move) {
    m_deltas.emplace_back(createDelta(pieceAt(getTo(move)), m_castling, 0, m_halfmoves));
    m_moves.emplace_back(move);

    setPiece(getTo(move), pieceAt(getFrom(move)));
    unsetPiece(getFrom(move));

    m_turn = m_turn == Turn::WHITE ? Turn::BLACK : Turn::WHITE;
}

void Position::undoMove() {
    if (m_deltas.empty() || m_moves.empty()) return;

    uint32_t delta = m_deltas.back();
    uint16_t move = m_moves.back();
    m_deltas.pop_back();
    m_moves.pop_back();

    Square from = getFrom(move);
    Square to = getTo(move);
    uint8_t flags = getFlags(move);

    setPiece(from, pieceAt(to));
    setPiece(to, getCaptured(delta));

    m_castling = getCastling(delta);
    m_halfmoves = getHalfmoves(delta);
}

uint64_t Position::perft(uint8_t depth) {
    std::vector<Move> moveList;
    moveList.reserve(BOARD_SIZE * BOARD_SIZE);

    uint64_t nodes = 0;

    if (depth == 0) return 1ULL;

    generateMoves(moveList);

    for (const auto &move : moveList) {
        doMove(move);
        if (!IsLegal()) {
            nodes += perft(depth - 1);
            undoMove();
        }
    }
    return nodes;
}
