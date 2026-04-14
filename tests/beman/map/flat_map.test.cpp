// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/map/flat_map.hpp>

#include <gtest/gtest.h>

#include <string>
#include <string_view>

namespace {

using beman::map::flat_map;

// ---------------------------------------------------------------------------
// Basic hit / miss
// ---------------------------------------------------------------------------

TEST(FlatMapGetTest, get_existing_key_returns_value) {
    flat_map<std::string, int> m{{"alice", 42}, {"bob", 7}};
    auto                       v = m.get("alice");
    ASSERT_TRUE(v.has_value());
    EXPECT_EQ(*v, 42);
}

TEST(FlatMapGetTest, get_missing_key_returns_nullopt) {
    flat_map<std::string, int> m{{"alice", 42}};
    EXPECT_FALSE(m.get("carol").has_value());
}

// ---------------------------------------------------------------------------
// Mutability through the returned reference
// ---------------------------------------------------------------------------

TEST(FlatMapGetTest, get_non_const_allows_mutation) {
    flat_map<std::string, int> m{{"alice", 42}};
    auto                       v = m.get("alice");
    ASSERT_TRUE(v.has_value());
    *v = 100;
    EXPECT_EQ(m.at("alice"), 100);
}

// ---------------------------------------------------------------------------
// const get
// ---------------------------------------------------------------------------

TEST(FlatMapGetTest, get_on_const_map_returns_const_ref) {
    const flat_map<std::string, int> m{{"alice", 42}};
    auto                             v = m.get("alice");
    ASSERT_TRUE(v.has_value());
    EXPECT_EQ(*v, 42);
    static_assert(std::is_same_v<decltype(v), beman::optional::optional<const int&>>);
}

TEST(FlatMapGetTest, get_on_const_map_missing_key_returns_nullopt) {
    const flat_map<std::string, int> m{{"alice", 42}};
    EXPECT_FALSE(m.get("carol").has_value());
}

// ---------------------------------------------------------------------------
// get never inserts
// ---------------------------------------------------------------------------

TEST(FlatMapGetTest, get_does_not_insert_on_miss) {
    flat_map<std::string, int> m{{"alice", 42}};
    m.get("carol");
    EXPECT_EQ(m.size(), 1u);
    EXPECT_EQ(m.count("carol"), 0u);
}

// ---------------------------------------------------------------------------
// Monadic operations
// ---------------------------------------------------------------------------

TEST(FlatMapGetTest, value_or_on_hit) {
    flat_map<std::string, int> m{{"alice", 42}};
    EXPECT_EQ(m.get("alice").value_or(0), 42);
}

TEST(FlatMapGetTest, value_or_on_miss) {
    flat_map<std::string, int> m{{"alice", 42}};
    EXPECT_EQ(m.get("carol").value_or(-1), -1);
}

// ---------------------------------------------------------------------------
// Heterogeneous lookup (Compare::is_transparent via std::less<void>)
// ---------------------------------------------------------------------------

TEST(FlatMapGetTest, heterogeneous_get_hit) {
    flat_map<std::string, int, std::less<>> m{{"alice", 42}};
    auto                                    v = m.get(std::string_view{"alice"});
    ASSERT_TRUE(v.has_value());
    EXPECT_EQ(*v, 42);
}

TEST(FlatMapGetTest, heterogeneous_get_miss) {
    flat_map<std::string, int, std::less<>> m{{"alice", 42}};
    EXPECT_FALSE(m.get(std::string_view{"carol"}).has_value());
}

TEST(FlatMapGetTest, heterogeneous_get_const) {
    const flat_map<std::string, int, std::less<>> m{{"alice", 42}};
    auto                                          v = m.get(std::string_view{"alice"});
    ASSERT_TRUE(v.has_value());
    static_assert(std::is_same_v<decltype(v), beman::optional::optional<const int&>>);
}

// ---------------------------------------------------------------------------
// Empty map
// ---------------------------------------------------------------------------

TEST(FlatMapGetTest, get_on_empty_map_returns_nullopt) {
    flat_map<int, int> m;
    EXPECT_FALSE(m.get(0).has_value());
}

} // namespace
