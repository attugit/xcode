#pragma once
#include <utility>
#include <type_traits>
#include <boost/iterator/iterator_facade.hpp>

namespace xcode
{
  struct node_links {
    node_links* prev = nullptr;
    node_links* next = nullptr;
    node_links* parent = nullptr;
    node_links* children = nullptr;

    void hook_after(node_links&);

    void hook_under(node_links&);

    void bind_after(node_links&);

    void unlink();
  };

  template <typename T>
  struct node : node_links {
    using value_type = T;
    value_type value;

    template <typename... Args>
    explicit node(Args&&... args) : value(std::forward<Args>(args)...)
    {
    }
  };

  template <typename T>
  struct sibling_iterator
      : boost::iterator_facade<sibling_iterator<T>, T, boost::bidirectional_traversal_tag> {
  private:
    struct enabler {
    };

  public:
    sibling_iterator() = default;

    template <typename U>
    explicit sibling_iterator(
        node<U>& n,
        typename std::enable_if<std::is_convertible<U, T>::value, enabler>::type = enabler{})
        : link(static_cast<node_links*>(&n))
    {
    }

    template <typename U>
    sibling_iterator(
        sibling_iterator<U> const& other,
        typename std::enable_if<std::is_convertible<U, T>::value, enabler>::type = enabler{})
        : link(other.link)
    {
    }

  private:
    friend class boost::iterator_core_access;
    template <typename>
    friend struct sibling_iterator;

    template <typename U>
    bool equal(sibling_iterator<U> const& other) const
    {
      return link == other.link;
    }

    void increment() { link = link->next; }
    void decrement() { link = link->prev; }
    T& dereference() const { return static_cast<node<T>*>(link)->value; }
    node_links* link = nullptr;
  };
}
