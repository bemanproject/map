// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef BEMAN_MAP_MAP_HPP
#define BEMAN_MAP_MAP_HPP

// beman::map::map — std::map extended with P3091 .get() lookups.
//
// P3091R5: Better lookups for map, unordered_map, and flat_map (Pablo Halpern)
// https://wg21.link/P3091R5
//
// New member functions (all constexpr, noexcept):
//
//   optional<mapped_type&>       get(const key_type& k);
//   optional<const mapped_type&> get(const key_type& k) const;
//
//   template<class K>  // requires Compare::is_transparent
//   optional<mapped_type&>       get(const K& k);
//   template<class K>  // requires Compare::is_transparent
//   optional<const mapped_type&> get(const K& k) const;

#include <beman/optional/optional.hpp>

#include <functional>
#include <map>
#include <memory>

namespace beman::map {

template <class Key,
          class T,
          class Compare   = std::less<Key>,
          class Allocator = std::allocator<std::pair<const Key, T>>>
class map : public std::map<Key, T, Compare, Allocator> {
    using base_type = std::map<Key, T, Compare, Allocator>;

  public:
    // Inherit all constructors from std::map.
    using base_type::base_type;

    // --- non-const get ---

    /// Returns an optional reference to the mapped value for key k.
    /// Returns nullopt if k is not present. Never throws, never inserts.
    constexpr beman::optional::optional<T&> get(const Key& k) noexcept {
        auto it = this->find(k);
        if (it == this->end())
            return beman::optional::nullopt;
        return it->second;
    }

    /// Heterogeneous overload: available when Compare::is_transparent is defined.
    template <class K>
        requires requires { typename Compare::is_transparent; }
    constexpr beman::optional::optional<T&> get(const K& k) noexcept {
        auto it = this->find(k);
        if (it == this->end())
            return beman::optional::nullopt;
        return it->second;
    }

    // --- const get ---

    /// Returns an optional const reference to the mapped value for key k.
    /// Returns nullopt if k is not present.
    constexpr beman::optional::optional<const T&> get(const Key& k) const noexcept {
        auto it = this->find(k);
        if (it == this->end())
            return beman::optional::nullopt;
        return it->second;
    }

    /// Heterogeneous overload: available when Compare::is_transparent is defined.
    template <class K>
        requires requires { typename Compare::is_transparent; }
    constexpr beman::optional::optional<const T&> get(const K& k) const noexcept {
        auto it = this->find(k);
        if (it == this->end())
            return beman::optional::nullopt;
        return it->second;
    }
};

} // namespace beman::map

#endif // BEMAN_MAP_MAP_HPP
