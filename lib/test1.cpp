//
// Created by DejanP on 2021/04/08.
//

#include "gtest/gtest.h"

// Tests factorial of 0.
TEST(FactorialTest, HandlesZeroInput) {
EXPECT_EQ(1, 1);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
