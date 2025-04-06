#include "navigation.hpp"

#include <gtest/gtest.h>

#include <cstdint>

TEST(NavigationTest, TurnPos) {
    EXPECT_EQ(getTurnSquare(uint8_t(0), WHITE), 0);
    EXPECT_EQ(getTurnSquare(uint8_t(1), WHITE), 1);
    EXPECT_EQ(getTurnSquare(uint8_t(0), BLACK), 63);
    EXPECT_EQ(getTurnSquare(uint8_t(1), BLACK), 62);
}
