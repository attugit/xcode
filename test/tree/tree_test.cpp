#include "src/lib/tree.hpp"
#include <gtest/gtest.h>

namespace
{
  using namespace xcode;
  struct node_links_test {
  };

  TEST(node_links_test, canHookUpNodes)
  {
    node_links n0, n1, n2, n3;
    EXPECT_FALSE(n0.parent);
    EXPECT_FALSE(n0.children);
    EXPECT_FALSE(n0.prev);
    EXPECT_FALSE(n0.next);

    n0.bind_after(n0);
    EXPECT_FALSE(n0.parent);
    EXPECT_FALSE(n0.children);
    EXPECT_EQ(n0.next, &n0);
    EXPECT_EQ(n0.prev, &n0);

    n1.hook_under(n0);
    EXPECT_EQ(n1.parent, &n0);
    EXPECT_EQ(n0.children, &n1);
    EXPECT_FALSE(n1.prev);
    EXPECT_FALSE(n1.next);

    n1.bind_after(n1);
    EXPECT_EQ(n1.prev, &n1);
    EXPECT_EQ(n1.next, &n1);

    n2.hook_after(n1);
    EXPECT_EQ(n0.children, &n1);
    EXPECT_EQ(n1.parent, &n0);
    EXPECT_EQ(n2.parent, &n0);
    EXPECT_EQ(n1.prev, &n2);
    EXPECT_EQ(n2.next, &n1);
    EXPECT_EQ(n1.next, &n2);
    EXPECT_EQ(n2.prev, &n1);

    n3.hook_after(n1);
    EXPECT_EQ(n0.children, &n1);
    EXPECT_EQ(n1.parent, &n0);
    EXPECT_EQ(n2.parent, &n0);
    EXPECT_EQ(n3.parent, &n0);
    EXPECT_EQ(n1.prev, &n2);
    EXPECT_EQ(n2.next, &n1);
    EXPECT_EQ(n1.next, &n3);
    EXPECT_EQ(n3.prev, &n1);
    EXPECT_EQ(n3.next, &n2);
    EXPECT_EQ(n2.prev, &n3);

    n3.unlink();
    EXPECT_FALSE(n3.next);
    EXPECT_FALSE(n3.prev);
    EXPECT_FALSE(n3.parent);
    EXPECT_EQ(n0.children, &n1);
    EXPECT_EQ(n1.parent, &n0);
    EXPECT_EQ(n2.parent, &n0);
    EXPECT_EQ(n1.prev, &n2);
    EXPECT_EQ(n2.next, &n1);
    EXPECT_EQ(n1.next, &n2);
    EXPECT_EQ(n2.prev, &n1);

    n1.unlink();
    EXPECT_FALSE(n1.next);
    EXPECT_FALSE(n1.prev);
    EXPECT_FALSE(n1.parent);
    EXPECT_EQ(n0.children, &n2);
    EXPECT_EQ(n2.next, &n2);
    EXPECT_EQ(n2.prev, &n2);

    n2.unlink();
    EXPECT_FALSE(n2.next);
    EXPECT_FALSE(n2.prev);
    EXPECT_FALSE(n2.parent);
    EXPECT_FALSE(n0.children);
  }

  struct sibling_iterator_test : ::testing::Test {
  };

  TEST(sibling_iterator_test, canConstructDefault)
  {
    sibling_iterator<int> it;
    (void)it;
  }

  TEST(sibling_iterator_test, canConstructWithNode)
  {
    node<int> n(7);
    sibling_iterator<int> it(n);
    (void)it;
  }

  TEST(sibling_iterator_test, canRValueDereference)
  {
    node<int> n(7);
    sibling_iterator<int> it(n);
    EXPECT_EQ(*it, 7);
  }

  TEST(sibling_iterator_test, canLValueDereference)
  {
    node<int> n(7);
    sibling_iterator<int> it(n);
    *it = 44;
    EXPECT_EQ(n.value, 44);
  }

  TEST(sibling_iterator_test, iteratorInteroperability)
  {
    node<int> n(7);
    sibling_iterator<const int> cit(n);
    EXPECT_EQ(*cit, 7);

    sibling_iterator<int> it(n);
    sibling_iterator<const int> cit2(it);
    EXPECT_EQ(cit2, it);
    EXPECT_EQ(*cit2, 7);
    *it = 44;
    EXPECT_EQ(*cit2, 44);
  }
}
