#include "inc/xcode/node.hpp"
#include <gtest/gtest.h>

namespace
{
  using namespace xcode;
  struct node_links_test {
  };

  TEST(node_links_test, canHookUpNodes)
  {
    xcode::link n0, n1, n2, n3;

    EXPECT_FALSE(n0.children);
    EXPECT_EQ(n0.parent, &n0);
    EXPECT_EQ(n0.next, &n0);
    EXPECT_EQ(n0.prev, &n0);

    n1.parent = &n0;
    n0.children = &n1;
    EXPECT_EQ(n1.prev, &n1);
    EXPECT_EQ(n1.next, &n1);

    n2.hook(n1);
    EXPECT_EQ(n0.children, &n1);
    EXPECT_EQ(n1.parent, &n0);
    EXPECT_EQ(n2.parent, &n0);
    EXPECT_EQ(n1.prev, &n2);
    EXPECT_EQ(n2.next, &n1);
    EXPECT_EQ(n1.next, &n2);
    EXPECT_EQ(n2.prev, &n1);

    n3.hook(n1);
    EXPECT_EQ(n0.children, &n1);
    EXPECT_EQ(n1.parent, &n0);
    EXPECT_EQ(n2.parent, &n0);
    EXPECT_EQ(n3.parent, &n0);
    EXPECT_EQ(n2.next, &n3);
    EXPECT_EQ(n3.next, &n1);
    EXPECT_EQ(n1.next, &n2);
    EXPECT_EQ(n1.prev, &n3);
    EXPECT_EQ(n3.prev, &n2);
    EXPECT_EQ(n2.prev, &n1);

    n3.unhook();
    EXPECT_EQ(n3.parent, &n3);
    EXPECT_EQ(n3.next, &n3);
    EXPECT_EQ(n3.prev, &n3);
    EXPECT_EQ(n0.children, &n1);
    EXPECT_EQ(n1.parent, &n0);
    EXPECT_EQ(n2.parent, &n0);
    EXPECT_EQ(n1.prev, &n2);
    EXPECT_EQ(n2.next, &n1);
    EXPECT_EQ(n1.next, &n2);
    EXPECT_EQ(n2.prev, &n1);

    n1.unhook();
    EXPECT_EQ(n1.parent, &n1);
    EXPECT_EQ(n1.next, &n1);
    EXPECT_EQ(n1.prev, &n1);
    EXPECT_EQ(n0.children, &n2);
    EXPECT_EQ(n2.next, &n2);
    EXPECT_EQ(n2.prev, &n2);

    n2.unhook();
    EXPECT_EQ(n2.parent, &n2);
    EXPECT_EQ(n2.next, &n2);
    EXPECT_EQ(n2.prev, &n2);
    EXPECT_FALSE(n0.children);
  }
}
