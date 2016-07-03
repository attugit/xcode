#include "inc/xcode/siblings.hpp"
#include <gtest/gtest.h>

namespace
{
  using namespace xcode;
  struct sibling_iterator_test : ::testing::Test {
  };

  TEST(sibling_iterator_test, canConstructDefault)
  {
    siblings<int>::iterator it;
    (void)it;
  }

  TEST(sibling_iterator_test, canConstructWithNode)
  {
    node<int> n(7);
    siblings<int>::iterator it(&n);
    (void)it;
  }

  TEST(sibling_iterator_test, canRValueDereference)
  {
    node<int> n(7);
    siblings<int>::iterator it(&n);
    EXPECT_EQ(*it, 7);
  }

  TEST(sibling_iterator_test, canLValueDereference)
  {
    node<int> n(7);
    siblings<int>::iterator it(&n);
    *it = 44;
    EXPECT_EQ(n.value, 44);
  }

  TEST(sibling_iterator_test, interoperabilityCopyCtorFromDefault)
  {
    siblings<int>::iterator it;
    siblings<int>::const_iterator cit(it);
    EXPECT_EQ(cit, it);
  }

  TEST(sibling_iterator_test, interoperabilityCopyCtorFromCustom)
  {
    node<int> n(7);
    siblings<int>::iterator it(&n);
    siblings<int>::const_iterator cit(it);
    EXPECT_EQ(cit, it);
    EXPECT_EQ(*cit, 7);
    *it = 44;
    EXPECT_EQ(*cit, 44);
  }

  TEST(sibling_iterator_test, interoperabilityCtor)
  {
    node<int> n(7);
    siblings<int>::const_iterator cit(&n);
    EXPECT_EQ(*cit, 7);
  }

  TEST(sibling_iterator_test, constIteratorCtor)
  {
    const node<int> n(7);
    siblings<int>::const_iterator cit(&n);
    EXPECT_EQ(*cit, 7);
  }
}
