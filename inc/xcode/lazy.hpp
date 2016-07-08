#pragma once

#include <functional>
#include <experimental/optional>

namespace xcode
{
  template <typename T>
  struct lazy {
    using value_type = T;
    using reference = value_type&;
    using const_reference = value_type const&;
    using pointer = value_type*;
    using const_pointer = value_type const*;
    using optional_t = std::experimental::optional<value_type>;

    template <typename... Args>
    explicit lazy(Args... args)
        : delegate([args...](optional_t& o) { o.emplace(args...); }),
          opt(std::experimental::nullopt)
    {
    }

    reference operator*()
    {
      return get();
    }

    const_reference operator*() const
    {
      return get();
    }

    pointer operator->()
    {
      return &get();
    }

    const_pointer operator->() const
    {
      return &get();
    }

    operator reference()
    {
      return get();
    }

    operator const_reference() const
    {
      return get();
    }

  private:
    void late_init() const
    {
      if (!opt) delegate(const_cast<optional_t&>(opt));
    }

    reference get()
    {
      late_init();
      return opt.value();
    }

    const_reference get() const
    {
      late_init();
      return opt.value();
    }

    std::function<void(optional_t&)> const delegate;
    optional_t opt;
  };
}
