#include "inc/xcode/lazy.hpp"
#include <gtest/gtest.h>

namespace
{
  using namespace xcode;

  struct tester {
    enum
    {
      def,
      arg
    } ctor;
    static unsigned instances;
    tester() : ctor(def)
    {
      ++instances;
    }
    explicit tester(int, int) : ctor(arg)
    {
      ++instances;
    }
    ~tester()
    {
      --instances;
    }
  };

  unsigned tester::instances = 0u;

  struct lazy_test : ::testing::Test {
    void SetUp() override
    {
      ASSERT_EQ(tester::instances, 0u);
    };
    void TearDown() override
    {
      ASSERT_EQ(tester::instances, 0u);
    }
  };

  TEST(lazy_test, noCtorWhenUnused)
  {
    lazy<tester> l;
    EXPECT_EQ(tester::instances, 0u);
    if (false) {
      auto& x = *l;
      (void)x;
    }
    EXPECT_EQ(tester::instances, 0u);
  }

  TEST(lazy_test, canCallDefaultCtor)
  {
    lazy<tester> l;
    EXPECT_EQ(tester::instances, 0u);
    auto& x = *l;
    (void)x;
    EXPECT_EQ(tester::instances, 1u);
    EXPECT_EQ(l->ctor, tester::def);
    EXPECT_EQ(tester::instances, 1u);
  }

  TEST(lazy_test, canPassByValueArgsToCtor)
  {
    lazy<tester> l(7, 7);
    EXPECT_EQ(tester::instances, 0u);
    auto& x = *l;
    (void)x;
    EXPECT_EQ(tester::instances, 1u);
    EXPECT_EQ(l->ctor, tester::arg);
    EXPECT_EQ(tester::instances, 1u);
  }

  TEST(lazy_test, canPointerOperatorCanInitialize)
  {
    lazy<tester> l(7, 7);
    EXPECT_EQ(tester::instances, 0u);
    EXPECT_EQ(l->ctor, tester::arg);
    EXPECT_EQ(tester::instances, 1u);
  }

  TEST(lazy_test, canOperatorCastCanInitialize)
  {
    auto const foo = [](tester&) {};
    auto const cfoo = [](tester const&) {};
    lazy<tester> l;
    lazy<tester> const cl;
    EXPECT_EQ(tester::instances, 0u);
    foo(l);
    EXPECT_EQ(tester::instances, 1u);
    cfoo(cl);
    EXPECT_EQ(tester::instances, 2u);
  }
}
