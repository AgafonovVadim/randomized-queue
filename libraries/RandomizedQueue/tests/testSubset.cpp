#include <algorithm>
#include <iostream>
#include <sstream>

#include "Subset.hpp"
#include "gtest/gtest.h"

TEST(SubsetTest, zero_from_empty) {
    std::stringstream input, output;
    subset(0, input, output);
    const auto out = output.str();
    EXPECT_TRUE(out.empty());
}

TEST(SubsetTest, three_from_empty) {
    std::stringstream input, output;
    subset(3, input, output);
    const auto out = output.str();
    EXPECT_TRUE(out.empty());
}

TEST(SubsetTest, zero_from_one) {
    std::stringstream input, output;
    input << "One";
    subset(0, input, output);
    const auto out = output.str();
    EXPECT_TRUE(out.empty());
}

TEST(SubsetTest, three_from_one) {
    std::stringstream input, output;
    input << "One";
    subset(3, input, output);
    const auto out = output.str();
    ASSERT_FALSE(out.empty());
    EXPECT_GE(1, std::count(out.begin(), out.end(), '\n'));
}

TEST(SubsetTest, three_from_three) {
    std::stringstream input, output;
    input << "One\nTwo\nThree";
    subset(3, input, output);
    const auto out = output.str();
    ASSERT_FALSE(out.empty());
    const auto line_count = std::count(out.begin(), out.end(), '\n');
    EXPECT_LE(2, line_count);
    EXPECT_GE(3, line_count);
}
