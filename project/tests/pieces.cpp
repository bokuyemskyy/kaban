#include <gtest/gtest.h>

#include "piece.hpp"

TEST(Pieces, CreateGet) {
    EXPECT_EQ(Piece(Color::BLACK, PieceType::ROOK), Piece::B_ROOK);
    EXPECT_EQ(Piece(Color::WHITE, PieceType::PAWN), Piece::W_PAWN);

    EXPECT_EQ(Piece(Color::WHITE, PieceType::PAWN).color(), Color::WHITE);
    EXPECT_EQ(Piece(Color::BLACK, PieceType::PAWN).color(), Color::BLACK);

    EXPECT_EQ(Piece(Color::WHITE, PieceType::KING).pieceType(), PieceType::KING);
    EXPECT_EQ(Piece(Color::BLACK, PieceType::QUEEN).pieceType(), PieceType::QUEEN);
}