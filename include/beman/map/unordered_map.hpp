// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef BEMAN_MAP_UNORDERED_MAP_HPP
#define BEMAN_MAP_UNORDERED_MAP_HPP

// beman::map::unordered_map — std::unordered_map extended with P3091 .get() lookups.
//
// P3091R5: Better lookups for map, unordered_map, and flat_map (Pablo Halpern)
// https://wg21.link/P3091R5
//
// New member functions (all noexcept):
//
//   optional<mapped_type&>       get(const key_type& k);
//   optional<const mapped_type&> get(const key_type& k) const;
//
//   template<class K>  // requires Hash::is_transparent && KeyEqual::is_transparent
//   optional<mapped_type&>       get(const K& k);
//   template<class K>  // requires Hash::is_transparent && KeyEqual::is_transparent
//   optional<const mapped_type&> get(const K& k) const;

#include <beman/optional/optional.hpp>

#include <functional>
#include <memory>
#include <unordered_map>

namespace beman::map {

template <class Key,
          class T,
          class Hash      = std::hash<Key>,
          class KeyEqual  = std::equal_to<Key>,
          class Allocator = std::allocator<std::pair<const Key, T>>>
class unordered_map : public std::unordered_map<Key, T, Hash, KeyEqual, Allocator> {
    using base_type = std::unordered_map<Key, T, Hash, KeyEqual, Allocator>;

  public:
    using base_type::base_type;

    // --- non-const get ---

    /// Returns an optional reference to the mapped value for key k.
    /// Returns nullopt if k is not present. Never throws, never inserts.
    beman::optional::optional<T&> get(const Key& k) noexcept {
        auto it = this->find(k);
        if (it == this->end())
            return beman::optional::nullopt;
        return it->second;
    }

    /// Heterogeneous overload: requires Hash::is_transparent and KeyEqual::is_transparent.
    template <class K>
        requires requires {
            typename Hash::is_transparent;
            typename KeyEqual::is_transparent;
        }
    beman::optional::optional<T&> get(const K& k) noexcept {
        auto it = this->find(k);
        if (it == this->end())
            return beman::optional::nullopt;
        return it->second;
    }

    // --- const get ---

    /// Returns an optional const reference to the mapped value for key k.
    /// Returns nullopt if k is not present.
    beman::optional::optional<const T&> get(const Key& k) const noexcept {
        auto it = this->find(k);
        if (it == this->end())
            return beman::optional::nullopt;
        return it->second;
    }

    /// Heterogeneous overload: requires Hash::is_transparent and KeyEqual::is_transparent.
    template <class K>
        requires requires {
            typename Hash::is_transparent;
            typename KeyEqual::is_transparent;
        }
    beman::optional::optional<const T&> get(const K& k) const noexcept {
        auto it = this->find(k);
        if (it == this->end())
            return beman::optional::nullopt;
        return it->second;
    }
};

} // namespace beman::map

#endif // BEMAN_MAP_UNORDERED_MAP_HPP
