#include "move.hpp"

#include <gtest/gtest.h>

#include "castling.hpp"
#include "piece.hpp"
#include "square.hpp"

TEST(MoveTest, CreateAndExtractMove) {
    Square from  = Squares::A1;
    Square to    = Squares::H8;
    Flags  flags = 4;

    Move move = createMove(from, to, flags);

    EXPECT_EQ(getFrom(move), from);
    EXPECT_EQ(getTo(move), to);
    EXPECT_EQ(getFlags(move), flags);

    move = createMove(0, 0, 0);
    EXPECT_EQ(getFrom(move), 0);
    EXPECT_EQ(getTo(move), 0);
    EXPECT_EQ(getFlags(move), 0);
}

TEST(DeltaTest, CreateAndExtractDelta) {
    Piece    captured   = Pieces::WQUEEN;
    Castling castling   = Castlings::WKINGSIDE;
    uint8_t  enpassant  = 25;
    uint8_t  halfmoves  = 50;
    uint8_t  extraFlags = 2;

    Delta delta = createDelta(captured, castling, enpassant, halfmoves, extraFlags);

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
}