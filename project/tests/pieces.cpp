#include <gtest/gtest.h>

#include "piece.hpp"

TEST(Pieces, CreateGet) {
    EXPECT_EQ(createPiece(Color::BLACK, PieceType::ROOK), Piece::BROOK);
    EXPECT_EQ(createPiece(Color::WHITE, PieceType::PAWN), Piece::WPAWN);

    EXPECT_EQ(getColor(createPiece(Color::WHITE, PieceType::PAWN)), Color::WHITE);
    EXPECT_EQ(getColor(createPiece(Color::BLACK, PieceType::PAWN)), Color::BLACK);

    EXPECT_EQ(getPieceType(createPiece(Color::WHITE, PieceType::KING)), PieceType::KING);
    EXPECT_EQ(getPieceType(createPiece(Color::BLACK, PieceType::QUEEN)), PieceType::QUEEN);
}
