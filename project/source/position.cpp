#include "position.hpp"

#include <cassert>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

#include "move.hpp"
#include "square.hpp"
#include "types.hpp"

inline Square lsb(const Bitboard &x) { return static_cast<Square>(__builtin_ctzll(x)); }
inline Square popLsb(Bitboard &x) {
    Square i = lsb(x);
    x &= x - 1;
    return i;
}

void Position::setFromFen(const std::string &fen) {
    resetBoard();

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
            case 1:
                if (argument == "w")
                    m_turn = Color::WHITE;
                else
                    m_turn = Color::BLACK;
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
    m_bitboards[static_cast<u8>(p)] |= (1ULL << static_cast<u8>(s));
}

void Position::unsetPiece(Square s) {
    assert(s != Square::NONE);

    u64 mask = 1ULL << static_cast<u8>(s);

    for (u8 i = 0; i < PIECE_NB; ++i) {
        if ((m_bitboards[i] & mask) != 0ULL) {
            m_bitboards[i] &= ~mask;
            break;
        }
    }
}
void Position::resetBoard() {
    for (u8 i = 0; i < PIECE_NB; ++i) {
        m_bitboards[i] = 0ULL;
    }
    m_turn = Color::WHITE;
}

Piece Position::pieceAt(Square s) const {
    assert(s != Square::NONE);

    u64 mask = 1ULL << static_cast<u8>(s);

    for (u8 i = 0; i < PIECE_NB; ++i) {
        if ((m_bitboards[i] & mask) != 0ULL) return Piece(i);
    }
    return Piece::NONE;
}

bool Position::isEmpty(Square s) const { return pieceAt(s) == Piece::NONE; }
bool Position::isAlly(Square s) const {
    Piece p = pieceAt(s);
    return p != Piece::NONE && getColor(p) == m_turn;
}
bool Position::isOpponent(Square s) const {
    Piece p = pieceAt(s);
    return p != Piece::NONE && getColor(p) != m_turn;
}
bool Position::isOpponent(Square s, Piece p) const { return pieceAt(s) == p; }

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
    if (getCaptured(delta) != Piece::NONE)
        setPiece(to, getCaptured(delta));
    else
        unsetPiece(to);

    m_castling = getCastling(delta);
    m_halfmoves = getHalfmoves(delta);

    m_turn = static_cast<Color>(!static_cast<bool>(m_turn));
}

void Position::generatePawnMoves(std::vector<Move> &moves, Square s) const {
    Direction moveDirection = (m_turn == Color::WHITE) ? Direction(0, 1) : Direction(0, -1);
    std::vector<Direction> captureDirections =
        (m_turn == Color::WHITE) ? std::vector<Direction>{Direction(-1, 1), Direction(1, 1)}
                                 : std::vector<Direction>{Direction(-1, -1), Direction(1, -1)};

    Square singlePush = s + moveDirection;
    if (singlePush != Square::NONE && isEmpty(singlePush)) {
        moves.emplace_back(createMove(s, singlePush, 0));

        if (isPawnStartingRank(getRank(s), m_turn)) {
            Square doublePush = singlePush + moveDirection;
            if (doublePush != Square::NONE && isEmpty(doublePush)) {
                moves.emplace_back(createMove(s, doublePush, 0));
            }
        }
    }
    for (const auto &captureDirection : captureDirections) {
        Square destination = s + captureDirection;
        if (destination != Square::NONE && isOpponent(destination)) {
            moves.emplace_back(createMove(s, destination, 0));
        }
    }
}
void Position::generateKnightMoves(std::vector<Move> &moves, Square s) const {
    static std::vector<Direction> directions = {
        Direction(2, 1),   Direction(1, 2),   Direction(-1, 2), Direction(-2, 1),
        Direction(-2, -1), Direction(-1, -2), Direction(1, -2), Direction(2, -1)};

    for (const auto &direction : directions) {
        Square destination = s + direction;
        if (destination != Square::NONE && !isAlly(destination)) {
            moves.emplace_back(createMove(s, destination, 0));
        }
    }
}
void Position::generateBishopMoves(std::vector<Move> &moves, Square s) const {
    static std::vector<Direction> iterativeDirections = {Direction(1, 1), Direction(-1, 1),
                                                         Direction(-1, -1), Direction(1, -1)};

    for (const auto &iterativeDirection : iterativeDirections) {
        Square destination = s;
        while (true) {
            destination = destination + iterativeDirection;
            if (destination == Square::NONE) break;
            if (isAlly(destination)) break;
            if (isOpponent(destination)) {
                moves.emplace_back(createMove(s, destination, 0));
                break;
            }
            moves.emplace_back(createMove(s, destination, 0));
        }
    }
}

void Position::generateRookMoves(std::vector<Move> &moves, Square s) const {
    static std::vector<Direction> iterativeDirections = {Direction(1, 0), Direction(0, 1),
                                                         Direction(-1, 0), Direction(0, -1)};

    for (const auto &iterativeDirection : iterativeDirections) {
        Square destination = s;
        while (true) {
            destination = destination + iterativeDirection;
            if (destination == Square::NONE) break;
            if (isAlly(destination)) break;
            if (isOpponent(destination)) {
                moves.emplace_back(createMove(s, destination, 0));
                break;
            }
            moves.emplace_back(createMove(s, destination, 0));
        }
    }
}
void Position::generateQueenMoves(std::vector<Move> &moves, Square s) const {
    static std::vector<Direction> iterativeDirections = {
        Direction(1, 0), Direction(0, 1),  Direction(-1, 0),  Direction(0, -1),
        Direction(1, 1), Direction(-1, 1), Direction(-1, -1), Direction(1, -1)};

    for (const auto &iterativeDirection : iterativeDirections) {
        Square destination = s;
        while (true) {
            destination = destination + iterativeDirection;
            if (destination == Square::NONE) break;
            if (isAlly(destination)) break;
            if (isOpponent(destination)) {
                moves.emplace_back(createMove(s, destination, 0));
                break;
            }
            moves.emplace_back(createMove(s, destination, 0));
        }
    }
}
void Position::generateKingMoves(std::vector<Move> &moves, Square s) const {
    static std::vector<Direction> directions = {
        Direction(1, 0), Direction(0, 1),  Direction(-1, 0),  Direction(0, -1),
        Direction(1, 1), Direction(-1, 1), Direction(-1, -1), Direction(1, -1)};

    for (const auto &direction : directions) {
        Square destination = s + direction;
        if (destination != Square::NONE && !isAlly(destination)) {
            moves.emplace_back(createMove(s, destination, 0));
        }
    }
}

void Position::generatePseudoLegalMoves(std::vector<Move> &moves) {
    Bitboard pawns = m_bitboards[static_cast<u8>(createPiece(m_turn, PieceType::PAWN))];
    while (pawns != 0ULL) {
        Square s = popLsb(pawns);
        generatePawnMoves(moves, s);
    }
    Bitboard knights = m_bitboards[static_cast<u8>(createPiece(m_turn, PieceType::KNIGHT))];
    while (knights != 0ULL) {
        Square s = popLsb(knights);
        generateKnightMoves(moves, s);
    }
    Bitboard bishops = m_bitboards[static_cast<u8>(createPiece(m_turn, PieceType::BISHOP))];
    while (bishops != 0ULL) {
        Square s = popLsb(bishops);
        generateBishopMoves(moves, s);
    }
    Bitboard rooks = m_bitboards[static_cast<u8>(createPiece(m_turn, PieceType::ROOK))];
    while (rooks != 0ULL) {
        Square s = popLsb(rooks);
        generateRookMoves(moves, s);
    }
    Bitboard queens = m_bitboards[static_cast<u8>(createPiece(m_turn, PieceType::QUEEN))];
    while (queens != 0ULL) {
        Square s = popLsb(queens);
        generateQueenMoves(moves, s);
    }
    Bitboard kings = m_bitboards[static_cast<u8>(createPiece(m_turn, PieceType::KING))];
    while (kings != 0ULL) {
        Square s = popLsb(kings);
        generateKingMoves(moves, s);
    }
}

bool Position::isLegal() {
    if (m_turn == Color::BLACK) {
        Bitboard kingBB = m_bitboards[static_cast<u8>(Piece::WKING)];
        Square king = lsb(m_bitboards[static_cast<u8>(Piece::WKING)]);
        Square opKing = lsb(m_bitboards[static_cast<u8>(Piece::BKING)]);
        if (pieceAt(king + Direction(-1, 1)) == Piece::BPAWN ||
            pieceAt(king + Direction(1, 1)) == Piece::BPAWN)
            return false;
        if (abs(static_cast<int>(getRank(king) - getRank(opKing))) <= 2 &&
            abs(static_cast<int>(getFile(king) - getFile(opKing))) <= 2)
            return false;
        static std::vector<Direction> knightDirections = {
            Direction(2, 1),   Direction(1, 2),   Direction(-1, 2), Direction(-2, 1),
            Direction(-2, -1), Direction(-1, -2), Direction(1, -2), Direction(2, -1)};
        for (const auto &direction : knightDirections) {
            Square destination = king + direction;
            if (destination != Square::NONE && pieceAt(destination) == Piece::BKNIGHT) return false;
        }
        static std::vector<Direction> bishopDirections = {Direction(1, 1), Direction(-1, 1),
                                                          Direction(-1, -1), Direction(1, -1)};

        static std::vector<Direction> rookDirections = {Direction(1, 0), Direction(0, 1),
                                                        Direction(-1, 0), Direction(0, -1)};
        for (const auto &direction : bishopDirections) {
            for (u8 i = 1; king + direction * i != Square::NONE; i++) {
                Piece p = pieceAt(king + direction * i);
                if (p == Piece::BQUEEN || p == Piece::BBISHOP) {
                    return false;
                } else if (p != Piece::NONE) {
                    break;
                }
            }
        }
        for (const auto &direction : rookDirections) {
            for (u8 i = 1; king + direction * i != Square::NONE; i++) {
                Piece p = pieceAt(king + direction * i);
                if (p == Piece::BQUEEN || p == Piece::BROOK) {
                    return false;
                } else if (p != Piece::NONE) {
                    break;
                }
            }
        }
    } else {
        Bitboard kingBB = m_bitboards[static_cast<u8>(Piece::BKING)];
        Square king = lsb(m_bitboards[static_cast<u8>(Piece::BKING)]);
        Square opKing = lsb(m_bitboards[static_cast<u8>(Piece::WKING)]);
        if (pieceAt(king + Direction(-1, -1)) == Piece::WPAWN ||
            pieceAt(king + Direction(1, -1)) == Piece::WPAWN)
            return false;
        if (abs(static_cast<int>(getRank(king) - getRank(opKing))) <= 2 &&
            abs(static_cast<int>(getFile(king) - getFile(opKing))) <= 2)
            return false;
        static std::vector<Direction> knightDirections = {
            Direction(2, 1),   Direction(1, 2),   Direction(-1, 2), Direction(-2, 1),
            Direction(-2, -1), Direction(-1, -2), Direction(1, -2), Direction(2, -1)};
        for (const auto &direction : knightDirections) {
            Square destination = king + direction;
            if (destination != Square::NONE && pieceAt(destination) == Piece::WKNIGHT) return false;
        }
        static std::vector<Direction> bishopDirections = {Direction(1, 1), Direction(-1, 1),
                                                          Direction(-1, -1), Direction(1, -1)};

        static std::vector<Direction> rookDirections = {Direction(1, 0), Direction(0, 1),
                                                        Direction(-1, 0), Direction(0, -1)};
        for (const auto &direction : bishopDirections) {
            for (u8 i = 1; king + direction * i != Square::NONE; i++) {
                Piece p = pieceAt(king + direction * i);
                if (p == Piece::WQUEEN || p == Piece::WBISHOP) {
                    return false;
                } else if (p != Piece::NONE) {
                    break;
                }
            }
        }
        for (const auto &direction : rookDirections) {
            for (u8 i = 1; king + direction * i != Square::NONE; i++) {
                Piece p = pieceAt(king + direction * i);
                if (p == Piece::WQUEEN || p == Piece::WROOK) {
                    return false;
                } else if (p != Piece::NONE) {
                    break;
                }
            }
        }
    }
    return true;
}

int Position::perft(u8 depth, bool verbose) {
    std::vector<Move> moves;
    moves.reserve(BOARD_SQUARES);

    u64 nodes = 0;

    if (depth == 0) return 1ULL;

    generatePseudoLegalMoves(moves);

    for (const auto &move : moves) {
        doMove(move);
        if (isLegal()) {
            int toAdd = perft(depth - 1);
            if (verbose) {
                std::cout << squareToString(getFrom(move)) << squareToString(getTo(move)) << ": "
                          << toAdd << "\n";
            }
            nodes += toAdd;
        }
        undoMove();
    }
    return nodes;
}
