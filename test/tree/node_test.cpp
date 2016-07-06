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

    EXPECT_EQ(n0.children, &n0);
    EXPECT_EQ(n0.parent, &n0);
    EXPECT_EQ(n0.next, &n0);
    EXPECT_EQ(n0.prev, &n0);

    n1.parent = &n0;
    EXPECT_EQ(n1.prev, &n1);
    EXPECT_EQ(n1.next, &n1);

    n2.hook(n1);
    EXPECT_EQ(n0.children, &n0);
    EXPECT_EQ(n1.parent, &n0);
    EXPECT_EQ(n2.parent, &n0);
    EXPECT_EQ(n1.prev, &n2);
    EXPECT_EQ(n2.next, &n1);
    EXPECT_EQ(n1.next, &n2);
    EXPECT_EQ(n2.prev, &n1);

    n3.hook(n1);
    EXPECT_EQ(n0.children, &n0);
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
    EXPECT_EQ(n0.children, &n0);
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
    EXPECT_EQ(n0.children, &n0);
    EXPECT_EQ(n2.next, &n2);
    EXPECT_EQ(n2.prev, &n2);

    n2.unhook();
    EXPECT_EQ(n2.parent, &n2);
    EXPECT_EQ(n2.next, &n2);
    EXPECT_EQ(n2.prev, &n2);
    EXPECT_EQ(n0.children, &n0);
  }

  TEST(node_links_test, canSwapNodes)
  {
    xcode::link a, b, c, d;
    b.hook(c);
    a.hook(b);
    EXPECT_EQ(a.next, &b);
    EXPECT_EQ(b.next, &c);
    EXPECT_EQ(c.next, &a);
    EXPECT_EQ(c.prev, &b);
    EXPECT_EQ(b.prev, &a);
    EXPECT_EQ(a.prev, &c);
    EXPECT_EQ(d.next, &d);
    EXPECT_EQ(d.prev, &d);

    using std::swap;
    swap(b, d);

    EXPECT_EQ(b.next, &b);
    EXPECT_EQ(b.prev, &b);

    EXPECT_EQ(d.next, &c);
    EXPECT_EQ(d.prev, &a);

    EXPECT_EQ(a.next, &d);
    EXPECT_EQ(c.prev, &d);

    EXPECT_EQ(a.prev, &c);
    EXPECT_EQ(c.next, &a);
  }
}
