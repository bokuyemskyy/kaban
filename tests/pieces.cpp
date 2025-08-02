#include <gtest/gtest.h>

#include "piece.hpp"

TEST(Pieces, CreateGet) {
    EXPECT_EQ(Piece(Color::BLACK, PieceType::ROOK).value(), Piece::B_ROOK);
    EXPECT_EQ(Piece(Color::WHITE, PieceType::PAWN).value(), Piece::W_PAWN);

    EXPECT_EQ(Piece(Color::WHITE, PieceType::PAWN).color().value(), Color::WHITE);
    EXPECT_EQ(Piece(Color::BLACK, PieceType::PAWN).color().value(), Color::BLACK);

    EXPECT_EQ(Piece(Color::WHITE, PieceType::KING).pieceType().value(), PieceType::KING);
    EXPECT_EQ(Piece(Color::BLACK, PieceType::QUEEN).pieceType().value(), PieceType::QUEEN);
}