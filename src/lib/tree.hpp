#pragma once
#include <utility>
#include <type_traits>
#include <memory>
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

  template <typename T, typename Alloc = std::allocator<T>>
  struct siblings {
  private:
    template <typename ValueType, typename NodeType, typename LinkType>
    struct iterator_impl : boost::iterator_facade<iterator_impl<ValueType, NodeType, LinkType>,
                                                  ValueType,
                                                  boost::bidirectional_traversal_tag> {
    private:
      using node_type = NodeType;
      using link_type = LinkType;
      struct enabler {
      };

    public:
      using value_type = ValueType;
      iterator_impl() = default;

      template <typename L>
      explicit iterator_impl(
          L* n,
          typename std::enable_if<std::is_base_of<node_links, L>::value, enabler>::type = enabler{})
          : link(n)
      {
      }

      template <typename V, typename N, typename L>
      iterator_impl(iterator_impl<V, N, L> const& other,
                    typename std::enable_if<std::is_convertible<V, value_type>::value,
                                            enabler>::type = enabler{})
          : link(other.link)
      {
      }

    private:
      friend class boost::iterator_core_access;
      template <typename, typename, typename>
      friend struct iterator_impl;

      template <typename V, typename N, typename L>
      bool equal(iterator_impl<V, N, L> const& other) const
      {
        return link == other.link;
      }

      void increment() { link = link->next; }
      void decrement() { link = link->prev; }
      value_type& dereference() const { return static_cast<node_type*>(link)->value; }
      link_type* link = nullptr;
    };

  public:
    using iterator = iterator_impl<T, node<T>, node_links>;
    using const_iterator = iterator_impl<const T, const node<T>, const node_links>;
  };
}
