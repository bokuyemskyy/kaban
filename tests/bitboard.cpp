#include "bitboard.hpp"

#include <gtest/gtest.h>

#include "navigation.hpp"

TEST(BitboardsTest, RankFileDiagAntiDiag) {
    EXPECT_EQ(Bitboard::rank(Ranks::R4), Bitboard(0xff000000));
    EXPECT_EQ(Bitboard::file(Files::FH).value(), Bitboard(0x8080808080808080));
    EXPECT_EQ(Bitboard::diag(Squares::A3).value(), Bitboard(0x804020100804));
    EXPECT_EQ(Bitboard::antiDiag(Squares::A4).value(), Bitboard(0x1020408));
}

TEST(BitboardsTest, DestinationValidAndInvalid) {
    Square    square    = Squares::D4;
    Direction direction = Direction::N;
    EXPECT_EQ(Bitboard::destination(square, direction), Bitboard::square(square + direction));
    EXPECT_EQ(Bitboard::destination(square, direction.value() * 3), Bitboard::zero());
}
