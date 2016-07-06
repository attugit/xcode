#pragma once
#include <utility>

namespace xcode
{
  template <typename Cls, typename T>
  using memptr = T Cls::*;

  struct link {
    using memptr = xcode::memptr<link, link*>;
    link* prev;
    link* next;
    link* children;
    link* parent;

    link() noexcept;

    link(link const&) = delete;
    link(link&&) noexcept;

    link& operator=(link const&) = delete;
    link& operator=(link&&) noexcept;

    void hook(link&) noexcept;
    void unhook() noexcept;
    void swap(link&) noexcept;
    void reparent(link&) noexcept;
  };

  template <typename T>
  struct node : link {
    using value_type = T;
    value_type value;

    template <typename... Args>
    explicit node(Args&&... args) : link(), value(std::forward<Args>(args)...)
    {
    }
  };

  template <typename T>
  T& get(link& lnk)
  {
    return static_cast<node<T>&>(lnk).value;
  }

  template <typename T>
  T const& get(link const& lnk)
  {
    return static_cast<node<T> const&>(lnk).value;
  }
}
