#include "position.hpp"

#include <gtest/gtest.h>

#include "piece.hpp"
#include "square.hpp"

TEST(Position, SetUnset) {
    Position pos = Position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");

    EXPECT_EQ(pos.toFen(), "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");

    pos.setPiece(Squares::A1, Pieces::BKING);

    EXPECT_EQ(pos.toFen(), "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/kNBQKBNR");

    pos.setPiece(Squares::B4, Pieces::WPAWN);

    EXPECT_EQ(pos.toFen(), "rnbqkbnr/pppppppp/8/8/1P6/8/PPPPPPPP/kNBQKBNR");

    pos.unsetPiece(Squares::A8);

    EXPECT_EQ(pos.toFen(), "1nbqkbnr/pppppppp/8/8/1P6/8/PPPPPPPP/kNBQKBNR");
}

TEST(Position, Move) {
    Position pos = Position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");

    EXPECT_EQ(pos.toFen(), "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");

    pos.doMove(createMove(Squares::A1, Squares::A2, 0));

    EXPECT_EQ(pos.toFen(), "rnbqkbnr/pppppppp/8/8/8/8/RPPPPPPP/1NBQKBNR");

    pos.undoMove();

    EXPECT_EQ(pos.toFen(), "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");

    pos.undoMove();

    EXPECT_EQ(pos.toFen(), "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
}

TEST(Position, IsQuestions) {
    Position pos = Position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");

    pos.doMove(createMove(Squares::A1, Squares::A3, 0));

    EXPECT_EQ(isPawnPromotionRank(Ranks::R8, Colors::WHITE), true);
    EXPECT_EQ(isPawnPromotionRank(Ranks::R8, Colors::BLACK), false);
    EXPECT_EQ(isPawnPromotionRank(Ranks::R1, Colors::BLACK), true);
    EXPECT_EQ(isPawnPromotionRank(Ranks::R3, Colors::WHITE), false);

    EXPECT_EQ(isPawnStartingRank(Ranks::R2, Colors::WHITE), true);
    EXPECT_EQ(isPawnStartingRank(Ranks::R3, Colors::BLACK), false);
    EXPECT_EQ(isPawnStartingRank(Ranks::R7, Colors::BLACK), true);
    EXPECT_EQ(isPawnStartingRank(Ranks::R3, Colors::WHITE), false);
}
