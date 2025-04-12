#include "position.hpp"

#include <gtest/gtest.h>

#include "types.hpp"

TEST(Position, SetUnset) {
    Position pos = Position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");

    EXPECT_EQ(pos.getFEN(), "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");

    pos.setPiece(Square::A1, Piece::BKING);

    EXPECT_EQ(pos.getFEN(), "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/kNBQKBNR");

    pos.setPiece(Square::B4, Piece::WPAWN);

    EXPECT_EQ(pos.getFEN(), "rnbqkbnr/pppppppp/8/8/1P6/8/PPPPPPPP/kNBQKBNR");

    pos.unsetPiece(Square::A8);

    EXPECT_EQ(pos.getFEN(), "1nbqkbnr/pppppppp/8/8/1P6/8/PPPPPPPP/kNBQKBNR");
}

TEST(Position, Move) {
    Position pos = Position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");

    EXPECT_EQ(pos.getFEN(), "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");

    pos.makeMove(createMove(Square::A1, Square::A2, 0));

    EXPECT_EQ(pos.getFEN(), "rnbqkbnr/pppppppp/8/8/8/8/RPPPPPPP/1NBQKBNR");

    pos.unmakeMove();

    EXPECT_EQ(pos.getFEN(), "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");

    pos.unmakeMove();

    EXPECT_EQ(pos.getFEN(), "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
}
