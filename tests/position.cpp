#include "position.hpp"

#include <gtest/gtest.h>

#include "navigation.hpp"
#include "piece.hpp"

TEST(Position, Move) {
    Position pos = Position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");

    EXPECT_EQ(pos.toFEN(), "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");

    pos.doMove(Move(Squares::A1, Squares::A2, 0, 0));

    EXPECT_EQ(pos.toFEN(), "rnbqkbnr/pppppppp/8/8/8/8/RPPPPPPP/1NBQKBNR");

    pos.undoMove();

    EXPECT_EQ(pos.toFEN(), "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");

    pos.undoMove();

    EXPECT_EQ(pos.toFEN(), "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
}
