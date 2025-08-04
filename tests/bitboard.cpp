#include "bitboard.hpp"

#include <gtest/gtest.h>

#include "navigation.hpp"

TEST(BitboardsTest, RankFileDiagAntiDiag) {
    EXPECT_EQ(Bitboard::rank(Rank::R4), Bitboard(0xff000000));
    EXPECT_EQ(Bitboard::file(File::FH).value(), Bitboard(0x8080808080808080));
    EXPECT_EQ(Bitboard::diag(Square::A3).value(), Bitboard(0x804020100804));
    EXPECT_EQ(Bitboard::antiDiag(Square::A4).value(), Bitboard(0x1020408));
}

TEST(BitboardsTest, DestinationValidAndInvalid) {
    Square    square    = Square::D4;
    Direction direction = Direction::N;
    EXPECT_EQ(Bitboard::destination(square, direction), Bitboard::square(square + direction));
    EXPECT_EQ(Bitboard::destination(square, direction * 3), Bitboard::zero());
}
