#include <gtest/gtest.h>

#include "piece.hpp"

TEST(Pieces, CreateGet) {
    EXPECT_EQ(createPiece(Colors::BLACK, PieceTypes::ROOK), Pieces::BROOK);
    EXPECT_EQ(createPiece(Colors::WHITE, PieceTypes::PAWN), Pieces::WPAWN);

    EXPECT_EQ(getColor(createPiece(Colors::WHITE, PieceTypes::PAWN)), Colors::WHITE);
    EXPECT_EQ(getColor(createPiece(Colors::BLACK, PieceTypes::PAWN)), Colors::BLACK);

    EXPECT_EQ(getPieceType(createPiece(Colors::WHITE, PieceTypes::KING)), PieceTypes::KING);
    EXPECT_EQ(getPieceType(createPiece(Colors::BLACK, PieceTypes::QUEEN)), PieceTypes::QUEEN);
}