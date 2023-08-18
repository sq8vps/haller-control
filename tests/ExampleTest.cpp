#include <gtest/gtest.h>
#include "../src/UdpNode.hpp"

TEST(ExampleTest, failingTest)
{
    EXPECT_TRUE(2+2==4);
}

TEST(ExampleTest, passingTest)
{
    UdpNode node{};

    EXPECT_FALSE(2+2==3);
}

