#include "bitboard.hpp"

#include <gtest/gtest.h>

#include "navigation.hpp"

TEST(BitboardsTest, RankFileDiagAntiDiag) {
    EXPECT_EQ(Bitboard::rankBB(Rank::R4), 0xff000000);
    EXPECT_EQ(Bitboard::fileBB(File::FH), 0x8080808080808080);
    EXPECT_EQ(Bitboard::diagBB(Square::A3), 0x804020100804);
    EXPECT_EQ(Bitboard::antiDiagBB(Square::A4), 0x1020408);
}

TEST(BitboardsTest, SquareBBBasic) {
    EXPECT_EQ(Bitboard::squareBB(Square::A1), 1ULL << Square::A1);
    EXPECT_EQ(Bitboard::squareBB(Square::H8), 1ULL << Square::H8);
}

TEST(BitboardsTest, DestinationBBValidAndInvalid) {
    Square    square    = Square::D4;
    Direction direction = Direction::N;
    EXPECT_EQ(Bitboard::destinationBB(square, direction), Bitboard::squareBB(square + direction));
    EXPECT_EQ(Bitboard::destinationBB(square, direction * 3), Bitboard::zero());
}