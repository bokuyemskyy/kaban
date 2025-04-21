#ifndef PIECE_HPP
#define PIECE_HPP

#include "types.hpp"

enum class PieceType : u8 {
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING,
};
constexpr u8 PIECETYPE_NB = 6;

enum class Color : bool { WHITE, BLACK };
inline Color operator!(Color c) { return static_cast<Color>(!static_cast<bool>(c)); }

enum class Piece : u8 {
    WPAWN,
    WKNIGHT,
    WBISHOP,
    WROOK,
    WQUEEN,
    WKING,

    BPAWN,
    BKNIGHT,
    BBISHOP,
    BROOK,
    BQUEEN,
    BKING,

    NONE
};
constexpr u8 PIECE_NB = 12;

inline Piece createPiece(Color c, PieceType pt) {
    return static_cast<Piece>(static_cast<u8>(pt) + (static_cast<u8>(c) * PIECETYPE_NB));
}

inline Color getColor(Piece p) {
    assert(p != Piece::NONE);
    return static_cast<Color>((static_cast<u8>(p) % PIECETYPE_NB != static_cast<u8>(p)));
}

inline PieceType getPieceType(Piece p) {
    assert(p != Piece::NONE);
    return static_cast<PieceType>(static_cast<u8>(p) % PIECETYPE_NB);
}

constexpr Piece charToPiece(char c) {
    switch (c) {
        case 'P':
            return Piece::WPAWN;
        case 'N':
            return Piece::WKNIGHT;
        case 'B':
            return Piece::WBISHOP;
        case 'R':
            return Piece::WROOK;
        case 'Q':
            return Piece::WQUEEN;
        case 'K':
            return Piece::WKING;
        case 'p':
            return Piece::BPAWN;
        case 'n':
            return Piece::BKNIGHT;
        case 'b':
            return Piece::BBISHOP;
        case 'r':
            return Piece::BROOK;
        case 'q':
            return Piece::BQUEEN;
        case 'k':
            return Piece::BKING;
        default:
            return Piece::NONE;
    }
}

constexpr char pieceToChar(Piece p) {
    switch (p) {
        case Piece::WPAWN:
            return 'P';
        case Piece::WKNIGHT:
            return 'N';
        case Piece::WBISHOP:
            return 'B';
        case Piece::WROOK:
            return 'R';
        case Piece::WQUEEN:
            return 'Q';
        case Piece::WKING:
            return 'K';
        case Piece::BPAWN:
            return 'p';
        case Piece::BKNIGHT:
            return 'n';
        case Piece::BBISHOP:
            return 'b';
        case Piece::BROOK:
            return 'r';
        case Piece::BQUEEN:
            return 'q';
        case Piece::BKING:
            return 'k';
        default:
            return '?';
    }
}

#endif