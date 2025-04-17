#include "position.hpp"

#include <cassert>
#include <sstream>
#include <string>
#include <vector>

#include "types.hpp"

inline Square lsb(const Bitboard &x) { return static_cast<Square>(__builtin_ctzll(x)); }
inline Square popLsb(Bitboard &x) {
    Square i = lsb(x);
    x &= x - 1;
    return i;
}

void Position::setFromFen(const std::string &fen) {
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

std::string Position::toFen() const {
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
    m_bitboards.at(static_cast<u8>(p)) |= (1ULL << static_cast<u8>(s));
}

void Position::unsetPiece(Square s) {
    assert(s != Square::NONE);

    u64 mask = 1ULL << static_cast<u8>(s);

    for (u8 i = 0; i < PIECE_NB; ++i) {
        if ((m_bitboards.at(i) & mask) != 0ULL) {
            m_bitboards.at(i) &= ~mask;
            break;
        }
    }
}

Piece Position::pieceAt(Square s) const {
    assert(s != Square::NONE);

    u64 mask = 1ULL << static_cast<u8>(s);

    for (u8 i = 0; i < PIECE_NB; ++i) {
        if ((m_bitboards.at(i) & mask) != 0ULL) return Piece(i);
    }
    return Piece::NONE;
}

bool Position::isEmpty(Square s) const { return pieceAt(s) == Piece::NONE; }

void Position::doMove(const Move move) {
    m_deltas.emplace_back(createDelta(pieceAt(getTo(move)), m_castling, 0, m_halfmoves));
    m_moves.emplace_back(move);

    setPiece(getTo(move), pieceAt(getFrom(move)));
    unsetPiece(getFrom(move));

    m_turn = static_cast<Color>(!static_cast<bool>(m_turn));
}

void Position::undoMove() {
    if (m_deltas.empty() || m_moves.empty()) return;

    u32 delta = m_deltas.back();
    u16 move = m_moves.back();
    m_deltas.pop_back();
    m_moves.pop_back();

    Square from = getFrom(move);
    Square to = getTo(move);
    u8 flags = getFlags(move);

    setPiece(from, pieceAt(to));
    setPiece(to, getCaptured(delta));

    m_castling = getCastling(delta);
    m_halfmoves = getHalfmoves(delta);
}

void Position::generatePawnMoves(std::vector<Move> &moves, Square s) {
    u8 direction = (m_turn == Color::WHITE) ? BOARD_SIZE : -BOARD_SIZE;
}

void Position::generatePseudoLegalMoves(std::vector<Move> &moves) {
    Bitboard pawns = m_bitboards.at(static_cast<u8>(createPiece(m_turn, PieceType::PAWN)));
    while (pawns != 0ULL) {
        Square s = popLsb(pawns);
        generatePawnMoves(moves, s);
    }
}

bool Position::isLegal() { return true; }

u64 Position::perft(u8 depth) {
    std::vector<Move> moves;
    moves.reserve(BOARD_SQUARES);

    u64 nodes = 0;

    if (depth == 0) return 1ULL;

    generatePseudoLegalMoves(moves);

    for (const auto &move : moves) {
        doMove(move);
        if (!isLegal()) {
            nodes += perft(depth - 1);
            undoMove();
        }
    }
    return nodes;
}
