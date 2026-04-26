// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/map/unordered_map.hpp>

#include <gtest/gtest.h>

#include <functional>
#include <string>
#include <string_view>

namespace {

using beman::map::unordered_map;

// ---------------------------------------------------------------------------
// Basic hit / miss
// ---------------------------------------------------------------------------

TEST(UnorderedMapGetTest, get_existing_key_returns_value) {
    unordered_map<std::string, int> m{{"alice", 42}, {"bob", 7}};
    auto                            v = m.get("alice");
    ASSERT_TRUE(v.has_value());
    EXPECT_EQ(*v, 42);
}

TEST(UnorderedMapGetTest, get_missing_key_returns_nullopt) {
    unordered_map<std::string, int> m{{"alice", 42}};
    EXPECT_FALSE(m.get("carol").has_value());
}

// ---------------------------------------------------------------------------
// Mutability through the returned reference
// ---------------------------------------------------------------------------

TEST(UnorderedMapGetTest, get_non_const_allows_mutation) {
    unordered_map<std::string, int> m{{"alice", 42}};
    auto                            v = m.get("alice");
    ASSERT_TRUE(v.has_value());
    *v = 100;
    EXPECT_EQ(m.at("alice"), 100);
}

// ---------------------------------------------------------------------------
// const get
// ---------------------------------------------------------------------------

TEST(UnorderedMapGetTest, get_on_const_map_returns_const_ref) {
    const unordered_map<std::string, int> m{{"alice", 42}};
    auto                                  v = m.get("alice");
    ASSERT_TRUE(v.has_value());
    EXPECT_EQ(*v, 42);
    static_assert(std::is_same_v<decltype(v), beman::optional::optional<const int&>>);
}

TEST(UnorderedMapGetTest, get_on_const_map_missing_key_returns_nullopt) {
    const unordered_map<std::string, int> m{{"alice", 42}};
    EXPECT_FALSE(m.get("carol").has_value());
}

// ---------------------------------------------------------------------------
// get never inserts
// ---------------------------------------------------------------------------

TEST(UnorderedMapGetTest, get_does_not_insert_on_miss) {
    unordered_map<std::string, int> m{{"alice", 42}};
    m.get("carol");
    EXPECT_EQ(m.size(), 1u);
    EXPECT_EQ(m.count("carol"), 0u);
}

// ---------------------------------------------------------------------------
// Monadic operations
// ---------------------------------------------------------------------------

TEST(UnorderedMapGetTest, value_or_on_hit) {
    unordered_map<std::string, int> m{{"alice", 42}};
    EXPECT_EQ(m.get("alice").value_or(0), 42);
}

TEST(UnorderedMapGetTest, value_or_on_miss) {
    unordered_map<std::string, int> m{{"alice", 42}};
    EXPECT_EQ(m.get("carol").value_or(-1), -1);
}

// ---------------------------------------------------------------------------
// Heterogeneous lookup
// Requires a transparent Hash AND a transparent KeyEqual.
// Use std::hash<std::string_view> + std::equal_to<> for demonstration.
// ---------------------------------------------------------------------------

struct transparent_string_hash {
    using is_transparent = void;
    std::size_t operator()(std::string_view sv) const noexcept { return std::hash<std::string_view>{}(sv); }
    std::size_t operator()(const std::string& s) const noexcept { return std::hash<std::string_view>{}(s); }
};

TEST(UnorderedMapGetTest, heterogeneous_get_hit) {
    unordered_map<std::string, int, transparent_string_hash, std::equal_to<>> m;
    m.emplace("alice", 42);
    auto v = m.get(std::string_view{"alice"});
    ASSERT_TRUE(v.has_value());
    EXPECT_EQ(*v, 42);
}

TEST(UnorderedMapGetTest, heterogeneous_get_miss) {
    unordered_map<std::string, int, transparent_string_hash, std::equal_to<>> m;
    m.emplace("alice", 42);
    EXPECT_FALSE(m.get(std::string_view{"carol"}).has_value());
}

TEST(UnorderedMapGetTest, heterogeneous_get_const) {
    unordered_map<std::string, int, transparent_string_hash, std::equal_to<>> base;
    base.emplace("alice", 42);
    const auto& m = base;
    auto        v = m.get(std::string_view{"alice"});
    ASSERT_TRUE(v.has_value());
    static_assert(std::is_same_v<decltype(v), beman::optional::optional<const int&>>);
}

// ---------------------------------------------------------------------------
// Empty map
// ---------------------------------------------------------------------------

TEST(UnorderedMapGetTest, get_on_empty_map_returns_nullopt) {
    unordered_map<int, int> m;
    EXPECT_FALSE(m.get(0).has_value());
}

} // namespace
