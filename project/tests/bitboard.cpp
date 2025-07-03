#include "bitboard.hpp"

#include <gtest/gtest.h>

#include "direction.hpp"
#include "square.hpp"

TEST(BitboardsTest, RankFileDiagAntiDiag) {
    EXPECT_EQ(Bitboards::rankBB(Ranks::R4), 0xff000000);
    EXPECT_EQ(Bitboards::fileBB(Files::FH), 0x8080808080808080);
    EXPECT_EQ(Bitboards::diagBB(Squares::A3), 0x804020100804);
    EXPECT_EQ(Bitboards::antiDiagBB(Squares::A4), 0x1020408);
}

TEST(BitboardsTest, SquareBBBasic) {
    EXPECT_EQ(Bitboards::squareBB(Squares::A1), 1ULL << Squares::A1);
    EXPECT_EQ(Bitboards::squareBB(Squares::H8), 1ULL << Squares::H8);
}

TEST(BitboardsTest, DestinationBBValidAndInvalid) {
    Square    square    = Squares::D4;
    Direction direction = Directions::N;
    EXPECT_EQ(Bitboards::destinationBB(square, direction), Bitboards::squareBB(square + direction));
    EXPECT_EQ(Bitboards::destinationBB(square, direction * 3), Bitboards::ZERO);
}