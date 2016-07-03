#include "inc/xcode/node.hpp"
#include <cassert>

namespace
{
  bool is_first(xcode::link const& lnk) { return lnk.prev == &lnk; }
  bool is_last(xcode::link const& lnk) { return lnk.next == &lnk; }
}

xcode::link::link() : prev(this), next(this), children(nullptr), parent(nullptr)
{
}

void xcode::link::hook(link& prv)
{
  assert(is_first(*this));
  assert(is_last(*this));
  parent = prv.parent;
  next = prv.next;
  next->prev = handle_t{this};
  prv.next = handle_t{this};
  prev = handle_t{&prv};
}

void xcode::link::unhook()
{
  if (parent && parent->children == this) {
    parent->children = is_last(*this) ? nullptr : next;
  }
  next->prev = is_first(*this) ? next : prev;
  prev->next = is_last(*this) ? prev : next;
  next = this;
  prev = this;
  parent = nullptr;
}
