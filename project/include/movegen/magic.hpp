#ifndef MAGICS_HPP
#define MAGICS_HPP

#include <array>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <vector>

#include "bitboard.hpp"
#include "piece.hpp"

using Shift = uint8_t;

struct Magic {
    Bitboard magic;
    Shift    shift;
    Bitboard premask;
};

constexpr int BISHOP_MAX_OCCUPANCY_NB = 1 << 9;
constexpr int ROOK_MAX_OCCUPANCY_NB   = 1 << 12;

extern std::array<std::array<Magic, Squares::NB>, 2> magics;

extern std::array<std::array<std::vector<Bitboard>, Squares::NB>, 2> premaskOccupancies;

template <PieceType PIECE_TYPE>
constexpr const Magic& getMagic(Square square) noexcept {
    assert(PIECE_TYPE == PieceTypes::ROOK || PIECE_TYPE == PieceTypes::BISHOP);
    assert(square < Squares::NB);
    return magics[PIECE_TYPE - PieceTypes::BISHOP][square];
}

constexpr void setMagic(PieceType pieceType, Square square, Magic magic) noexcept {
    assert(pieceType == PieceTypes::ROOK || pieceType == PieceTypes::BISHOP);
    assert(square < Squares::NB);
    magics[pieceType - PieceTypes::BISHOP][square] = magic;
}

constexpr const std::vector<Bitboard>& getPremaskOccupancies(PieceType pieceType, Square square) noexcept {
    assert(pieceType == PieceTypes::ROOK || pieceType == PieceTypes::BISHOP);
    assert(square < Squares::NB);
    return premaskOccupancies[pieceType - PieceTypes::BISHOP][square];
}

constexpr Bitboard getPremask(PieceType pieceType, Square square) {
    switch (pieceType) {
        case PieceTypes::ROOK:
            return ((fileBB[getFile(square)] & ~(rankBB[Ranks::R1] | rankBB[Ranks::R8])) |
                    (rankBB[getRank(square)] & ~(fileBB[Files::FA] | fileBB[Files::FH]))) &
                   ~squareBB(square);
        case PieceTypes::BISHOP:
            return (diagBB[square] | antiDiagBB[square]) &
                   ~(fileBB[Files::FA] | fileBB[Files::FH] | rankBB[Ranks::R1] | rankBB[Ranks::R8]) & ~squareBB(square);
        default:
            throw "Invalid piece type";
    }
}

Bitboard createOccupancy(int index, Bitboard premask);
void     initPremaskOccupancies(Square square, Bitboard premask);
void     initMagics();

template <PieceType PIECE_TYPE>
constexpr int getIndexOfOccupancy(Square square, Bitboard occupancy) {
    assert(PIECE_TYPE == PieceTypes::ROOK || PIECE_TYPE == PieceTypes::BISHOP);
    assert(square < Squares::NB);
    const auto& entry = getMagic<PIECE_TYPE>(square);
    return static_cast<int>(((occupancy & entry.premask) * entry.magic) >> entry.shift);
}

#endif