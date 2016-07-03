#pragma once
#include <utility>

namespace xcode
{
  template <typename T>
  using handle = T*;

  struct link {
    using handle_t = handle<link>;
    handle_t prev;
    handle_t next;
    handle_t children;
    handle_t parent;

    link();
    void hook(link&);
    void unhook();
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
}
