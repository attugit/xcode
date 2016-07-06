#include "inc/xcode/node.hpp"
#include <cassert>

namespace
{
  bool is_cycled(xcode::link::memptr hdl, xcode::link const& lnk)
  {
    return lnk.*hdl == &lnk;
  }
  bool is_first(xcode::link const& lnk)
  {
    return is_cycled(&xcode::link::prev, lnk);
  }
  bool is_last(xcode::link const& lnk)
  {
    return is_cycled(&xcode::link::next, lnk);
  }

  void exchange(xcode::link::memptr hdl,
                xcode::link::memptr cntr,
                xcode::link& lhs,
                xcode::link& rhs)
  {
    auto const pass = [hdl, cntr](auto& lnk, auto& other, auto n) {
      auto ret = lnk.*hdl;
      lnk.*hdl = n != &other ? (n->*cntr = &lnk, n) : &lnk;
      return ret;
    };

    pass(lhs, rhs, pass(rhs, lhs, lhs.*hdl));
  }
}

xcode::link::link() noexcept : prev(this), next(this), children(this), parent(this)
{
}

void xcode::link::swap(link& lnk) noexcept
{
  exchange(&link::prev, &link::next, *this, lnk);
  exchange(&link::next, &link::prev, *this, lnk);
  exchange(&link::parent, &link::children, *this, lnk);
  exchange(&link::children, &link::parent, *this, lnk);
}

xcode::link::link(link&& lnk) noexcept : link()
{
  swap(lnk);
}

xcode::link& xcode::link::operator=(xcode::link&& lnk) noexcept
{
  swap(lnk);
  return *this;
}

void xcode::link::hook(link& nxt) noexcept
{
  if (!is_first(*this) || !is_last(*this)) {
    unhook();
  }
  parent = nxt.parent;
  prev = nxt.prev;
  prev->next = this;
  nxt.prev = this;
  next = &nxt;
}

void xcode::link::unhook() noexcept
{
  next->prev = is_first(*this) ? next : prev;
  prev->next = is_last(*this) ? prev : next;
  next = this;
  prev = this;
  parent = this;
}

void xcode::link::reparent(link& lnk) noexcept
{
  exchange(&link::children, &link::parent, *this, lnk);
}
