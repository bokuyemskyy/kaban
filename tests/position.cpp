#include "position.hpp"

#include <gtest/gtest.h>

#include "navigation.hpp"
#include "piece.hpp"

TEST(Position, SetUnset) {
    /*Position pos = Position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");

    EXPECT_EQ(pos.toFEN(), "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");

    pos.set(Square::A1, Piece::B_KING);

    EXPECT_EQ(pos.toFEN(), "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/kNBQKBNR");

    pos.set(Square::B4, Piece::W_PAWN);

    EXPECT_EQ(pos.toFEN(), "rnbqkbnr/pppppppp/8/8/1P6/8/PPPPPPPP/kNBQKBNR");

    pos.unset(Square::A8);

    EXPECT_EQ(pos.toFEN(), "1nbqkbnr/pppppppp/8/8/1P6/8/PPPPPPPP/kNBQKBNR");
*/
}

TEST(Position, Move) {
    Position pos = Position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");

    EXPECT_EQ(pos.toFEN(), "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");

    pos.doMove(Move(Square::A1, Square::A2, 0, 0));

    EXPECT_EQ(pos.toFEN(), "rnbqkbnr/pppppppp/8/8/8/8/RPPPPPPP/1NBQKBNR");

    pos.undoMove();

    EXPECT_EQ(pos.toFEN(), "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");

    pos.undoMove();

    EXPECT_EQ(pos.toFEN(), "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
}

TEST(Position, Promoting) {
    EXPECT_EQ(Rank(Rank::R8).pawnPromoting(Color::WHITE), true);
    EXPECT_EQ(Rank(Rank::R8).pawnPromoting(Color::BLACK), false);
    EXPECT_EQ(Rank(Rank::R1).pawnPromoting(Color::BLACK), true);
    EXPECT_EQ(Rank(Rank::R3).pawnPromoting(Color::WHITE), false);

    EXPECT_EQ(Rank(Rank::R2).pawnStarting(Color::WHITE), true);
    EXPECT_EQ(Rank(Rank::R3).pawnStarting(Color::BLACK), false);
    EXPECT_EQ(Rank(Rank::R7).pawnStarting(Color::BLACK), true);
    EXPECT_EQ(Rank(Rank::R3).pawnStarting(Color::WHITE), false);
}
