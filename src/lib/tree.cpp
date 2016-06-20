#include <cassert>
#include "src/lib/tree.hpp"

void xcode::node_links::bind_after(node_links& n)
{
  n.next = this;
  prev = &n;
}

void xcode::node_links::hook_after(node_links& n)
{
  assert(prev == nullptr);
  assert(next == nullptr);
  parent = n.parent;
  next = n.next;
  if (next) {
    next->prev = this;
  }
  bind_after(n);
}

void xcode::node_links::hook_under(node_links& n)
{
  assert(n.children == nullptr);
  parent = &n;
  n.children = this;
}

void xcode::node_links::unlink()
{
  if (parent && parent->children == this) {
    parent->children = next != this ? next : nullptr;
  }
  if (next) next->prev = prev;
  if (prev) prev->next = next;
  next = nullptr;
  prev = nullptr;
  parent = nullptr;
}
