#include "inc/xcode/siblings.hpp"
#include <gtest/gtest.h>

namespace
{
  using namespace xcode;
  struct siblings_test : ::testing::Test {
  };

  TEST(siblings_test, canDefaultConstruct)
  {
    siblings<int> sib;
    EXPECT_TRUE(sib.empty());
    EXPECT_EQ(sib.size(), 0u);
    EXPECT_EQ(sib.begin(), sib.end());
  }

  TEST(siblings_test, canEmplace)
  {
    siblings<int> sib;
    sib.emplace(sib.end(), 7);
    EXPECT_FALSE(sib.empty());
    EXPECT_EQ(sib.size(), 1u);
    EXPECT_NE(sib.begin(), sib.end());
    EXPECT_EQ(*sib.begin(), 7);
  }

  TEST(siblings_test, canErase)
  {
    siblings<int> sib;
    sib.emplace(sib.end(), 7);
    sib.erase(sib.begin());
    EXPECT_TRUE(sib.empty());
    EXPECT_EQ(sib.begin(), sib.end());
    EXPECT_EQ(sib.size(), 0u);
  }

  TEST(siblings_test, canEmplaceBack)
  {
    siblings<int> sib;
    sib.emplace_back(7);
    EXPECT_FALSE(sib.empty());
    EXPECT_EQ(sib.size(), 1u);
    sib.emplace_back(42);
    EXPECT_EQ(sib.size(), 2u);
  }
}
