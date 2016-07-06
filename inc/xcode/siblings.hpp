#pragma once
#include <utility>
#include <type_traits>
#include <memory>
#include <iterator>
#include <initializer_list>
#include <boost/iterator/iterator_facade.hpp>
#include <xcode/node.hpp>

namespace xcode
{
  template <typename T, typename Alloc = std::allocator<T>>
  struct siblings {
    using allocator_type = Alloc;

  private:
    using traits = std::allocator_traits<allocator_type>;

  public:
    using size_type = typename traits::size_type;
    using difference_type = typename traits::difference_type;
    using value_type = typename traits::value_type;
    using pointer = typename traits::pointer;
    using const_pointer = typename traits::const_pointer;
    using reference = value_type&;
    using const_reference = value_type const&;

  private:
    using node_allocator = typename traits::template rebind_alloc<node<T>>;
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
          typename std::enable_if<std::is_base_of<link, L>::value, enabler>::type = enabler{})
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
      template <typename, typename>
      friend struct siblings;
      friend class boost::iterator_core_access;
      template <typename, typename, typename>
      friend struct iterator_impl;

      template <typename V, typename N, typename L>
      bool equal(iterator_impl<V, N, L> const& other) const
      {
        return link == other.link;
      }

      void increment()
      {
        link = link->next;
      }
      void decrement()
      {
        link = link->prev;
      }
      value_type& dereference() const
      {
        return get<value_type>(*link);
      }
      link_type* link = nullptr;
    };

    struct impl_type : node_allocator {
      link root;
      size_type size = 0;
    } impl;

  public:
    using iterator = iterator_impl<value_type, node<value_type>, link>;
    using const_iterator = iterator_impl<const value_type, const node<value_type>, const link>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    siblings() = default;
    template <typename InputIt>
    siblings(InputIt first, InputIt last) : siblings()
    {
      for (; first != last; ++first) {
        emplace(end(), *first);
      }
    }
    siblings(std::initializer_list<value_type> init) : siblings()
    {
      for (auto& x : init) {
        emplace(end(), std::move(x));
      }
    }
    siblings(siblings const& sib) : siblings(std::begin(sib), std::end(sib))
    {
    }
    siblings(siblings&& sib) : siblings()
    {
      using std::swap;
      swap(sib.impl.root, impl.root);
      swap(sib.impl.size, impl.size);
      for (auto it = begin(); it != end(); ++it) {
        it.link->parent = link::handle_t{&impl.root};
      }
    }
    siblings& operator=(siblings const& sib)
    {
      clear();
      for (auto const& x : sib) {
        emplace(end(), x);
      }
      return *this;
    }
    ~siblings()
    {
      clear();
    }

    /// Element access
    reference front()
    {
      return get<value_type>(*impl.root.next);
    }
    const_reference front() const
    {
      return get<value_type>(*impl.root.next);
    }
    reference back()
    {
      return get<value_type>(*impl.root.prev);
    }
    const_reference back() const
    {
      return get<value_type>(*impl.root.prev);
    }

    /// Iterators
    iterator begin()
    {
      return iterator{impl.root.next};
    }
    const_iterator cbegin() const
    {
      return const_iterator{impl.root.next};
    }
    const_iterator begin() const
    {
      return cbegin();
    }
    iterator end()
    {
      return iterator{&impl.root};
    }
    const_iterator cend() const
    {
      return const_iterator{&impl.root};
    }
    const_iterator end() const
    {
      return cend();
    }
    reverse_iterator rbegin();
    const_reverse_iterator crbegin() const;
    const_reverse_iterator rbegin() const;
    reverse_iterator rend();
    const_reverse_iterator crend() const;
    const_reverse_iterator rend() const;

    /// Capacity
    bool empty() const
    {
      return begin() == end();
    }
    size_type size() const
    {
      return impl.size;
    }
    size_type max_size() const;

    /// Modifiers
    void clear()
    {
      while (!empty()) {
        erase(begin());
      }
    }
    iterator insert(const_iterator pos, const_reference val)
    {
      return emplace(pos, val);
    }
    iterator insert(const_iterator pos, value_type&& val)
    {
      return emplace(pos, std::move(val));
    }
    template <typename... Args>
    iterator emplace(const_iterator it, Args&&... args)
    {
      auto ptr = impl.allocate(1);
      new (ptr) node<T>(std::forward<Args>(args)...);
      ptr->hook(const_cast<link&>(*(it.link)));
      ++impl.size;
      return iterator{ptr};
    }
    void erase(iterator it)
    {
      auto ptr = static_cast<node<T>*>(it.link);
      ptr->unhook();
      --impl.size;
      ptr->~node<T>();
      impl.deallocate(ptr, 1);
    }
    template <typename... Args>
    void emplace_back(Args&&... args)
    {
      emplace(end(), std::forward<Args>(args)...);
    }
  };
}
