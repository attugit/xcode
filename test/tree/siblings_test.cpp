#include "inc/xcode/siblings.hpp"
#include <vector>
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

  TEST(siblings_test, canInitConstruct)
  {
    siblings<int> sib({7, 44});
    EXPECT_EQ(sib.size(), 2u);
    EXPECT_EQ(sib.front(), 7);
    EXPECT_EQ(sib.back(), 44);
  }

  TEST(siblings_test, canInitAssign)
  {
    siblings<int> sib = {7, 44};
    EXPECT_EQ(sib.size(), 2u);
    EXPECT_EQ(sib.front(), 7);
    EXPECT_EQ(sib.back(), 44);
  }

  TEST(siblings_test, canRangeConstruct)
  {
    std::vector<int> vec = {7, 44};
    siblings<int> sib(std::begin(vec), std::end(vec));
    EXPECT_EQ(sib.size(), 2u);
    EXPECT_EQ(sib.front(), 7);
    EXPECT_EQ(sib.back(), 44);
  }

  TEST(siblings_test, canCopyConstruct)
  {
    siblings<int> const orig = {7, 44};
    siblings<int> sib(orig);
    EXPECT_EQ(sib.size(), 2u);
    EXPECT_EQ(sib.front(), 7);
    EXPECT_EQ(sib.back(), 44);
  }

  /// Element access
  TEST(siblings_test, canGetFront)
  {
    siblings<int> sib;
    sib.emplace(sib.end(), 7);
    EXPECT_EQ(sib.front(), 7);
    sib.front() = 44;
    EXPECT_EQ(*sib.begin(), 44);
    ASSERT_EQ(sib.size(), 1u);
    EXPECT_EQ(sib.front(), sib.back());
  }

  TEST(siblings_test, canEmplace)
  {
    siblings<int> sib;
    auto it = sib.emplace(sib.end(), 7);
    EXPECT_FALSE(sib.empty());
    EXPECT_EQ(sib.size(), 1u);
    EXPECT_NE(sib.begin(), sib.end());
    EXPECT_EQ(*sib.begin(), 7);
    EXPECT_EQ(*it, 7);
    it = sib.emplace(it, 44);
    EXPECT_EQ(*it, 44);
    EXPECT_EQ(sib.front(), 44);
    EXPECT_EQ(sib.back(), 7);
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
