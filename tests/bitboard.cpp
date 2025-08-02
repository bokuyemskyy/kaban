#include "bitboard.hpp"

#include <gtest/gtest.h>

#include "navigation.hpp"

TEST(BitboardsTest, RankFileDiagAntiDiag) {
    EXPECT_EQ(Bitboard::rankBB(Rank::R4), Bitboard(0xff000000));
    EXPECT_EQ(Bitboard::fileBB(File::FH).value(), Bitboard(0x8080808080808080));
    EXPECT_EQ(Bitboard::diagBB(Square::A3).value(), Bitboard(0x804020100804));
    EXPECT_EQ(Bitboard::antiDiagBB(Square::A4).value(), Bitboard(0x1020408));
}

TEST(BitboardsTest, DestinationBBValidAndInvalid) {
    Square    square    = Square::D4;
    Direction direction = Direction::N;
    EXPECT_EQ(Bitboard::destinationBB(square, direction), Bitboard::squareBB(square + direction));
    EXPECT_EQ(Bitboard::destinationBB(square, direction * 3), Bitboard::zero());
}