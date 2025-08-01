#include "move.hpp"

#include <gtest/gtest.h>

#include "navigation.hpp"

TEST(MoveTest, CreateAndExtractMove) {
    Square from  = Square::A1;
    Square to    = Square::H8;
    Flag   flags = 4;

    Move move = Move(from, to, flags, 0);

    EXPECT_EQ(move.from(), from);
    EXPECT_EQ(move.to(), to);
    EXPECT_EQ(move.flag(), flags);

    move = Move(0, 0, 0, 0);
    EXPECT_EQ(move.from(), 0);
    EXPECT_EQ(move.to(), 0);
    EXPECT_EQ(move.flag(), 0);
}
/*
TEST(DeltaTest, CreateAndExtractDelta) {
    Piece    captured   = Piece::W_QUEEN;
    Castling castling   = Castling::W_KING_SIDE;
    uint8_t  enpassant  = 25;
    uint8_t  halfmoves  = 50;
    uint8_t  extraFlags = 2;

    Delta delta = Delta(captured, castling, enpassant, halfmoves, extraFlags);

    EXPECT_EQ(getCaptured(delta), captured);
    EXPECT_EQ(getCastling(delta), castling);
    EXPECT_EQ(getEnpassant(delta), enpassant);
    EXPECT_EQ(getHalfmoves(delta), halfmoves);
    EXPECT_EQ(getExtraFlags(delta), extraFlags);

    delta = createDelta(0, 0, 0, 0, 0);
    EXPECT_EQ(getCaptured(delta), 0);
    EXPECT_EQ(getCastling(delta), 0);
    EXPECT_EQ(getEnpassant(delta), 0);
    EXPECT_EQ(getHalfmoves(delta), 0);
    EXPECT_EQ(getExtraFlags(delta), 0);
}*/