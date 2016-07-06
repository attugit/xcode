#include "inc/xcode/node.hpp"
#include <cassert>

namespace
{
  bool is_cycled(xcode::link::handle_t xcode::link::*hdl, xcode::link const& lnk)
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
  void pass(xcode::link::handle_t xcode::link::*hdl,
            xcode::link::handle_t xcode::link::*cntr,
            xcode::link::handle_t n,
            xcode::link& lnk,
            xcode::link& other)
  {
    if (n != xcode::link::handle_t{&other}) {
      lnk.*hdl = n;
      if (cntr) lnk.*hdl->*cntr = xcode::link::handle_t{&lnk};
    }
    else
    {
      lnk.*hdl = xcode::link::handle_t{&lnk};
    }
  }

  void exchange(xcode::link::handle_t xcode::link::*hdl,
                xcode::link::handle_t xcode::link::*cntr,
                xcode::link& lhs,
                xcode::link& rhs)
  {
    auto rhdl = rhs.*hdl;

    pass(hdl, cntr, lhs.*hdl, rhs, lhs);
    pass(hdl, cntr, rhdl, lhs, rhs);
  }
}

xcode::link::link() noexcept : prev(this), next(this), children(this), parent(this)
{
}

void xcode::link::swap(link& lnk) noexcept
{
  exchange(&link::prev, &link::next, *this, lnk);
  exchange(&link::next, &link::prev, *this, lnk);
  exchange(&link::parent, nullptr, *this, lnk);
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
  prev->next = handle_t{this};
  nxt.prev = handle_t{this};
  next = handle_t{&nxt};
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
  auto tmp = lnk.children != &lnk ? lnk.children : this;
  lnk.children = children != this ? children : &lnk;
  children = tmp;
  lnk.children->parent = &lnk;
  children->parent = this;
}
