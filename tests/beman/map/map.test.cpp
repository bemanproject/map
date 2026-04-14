// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#include <beman/map/map.hpp>

#include <gtest/gtest.h>

#include <string>
#include <string_view>

namespace {

using beman::map::map;

// ---------------------------------------------------------------------------
// Basic hit / miss
// ---------------------------------------------------------------------------

TEST(MapGetTest, get_existing_key_returns_value) {
    map<std::string, int> m{{"alice", 42}, {"bob", 7}};
    auto                  v = m.get("alice");
    ASSERT_TRUE(v.has_value());
    EXPECT_EQ(*v, 42);
}

TEST(MapGetTest, get_missing_key_returns_nullopt) {
    map<std::string, int> m{{"alice", 42}};
    auto                  v = m.get("carol");
    EXPECT_FALSE(v.has_value());
}

// ---------------------------------------------------------------------------
// Mutability through the returned reference
// ---------------------------------------------------------------------------

TEST(MapGetTest, get_non_const_allows_mutation) {
    map<std::string, int> m{{"alice", 42}};
    auto                  v = m.get("alice");
    ASSERT_TRUE(v.has_value());
    *v = 100;
    EXPECT_EQ(m.at("alice"), 100);
}

// ---------------------------------------------------------------------------
// const get
// ---------------------------------------------------------------------------

TEST(MapGetTest, get_on_const_map_returns_const_ref) {
    const map<std::string, int> m{{"alice", 42}};
    auto                        v = m.get("alice");
    ASSERT_TRUE(v.has_value());
    EXPECT_EQ(*v, 42);
    // v is optional<const int&> — the type check below must compile:
    static_assert(std::is_same_v<decltype(v), beman::optional::optional<const int&>>);
}

TEST(MapGetTest, get_on_const_map_missing_key_returns_nullopt) {
    const map<std::string, int> m{{"alice", 42}};
    EXPECT_FALSE(m.get("carol").has_value());
}

// ---------------------------------------------------------------------------
// get never inserts
// ---------------------------------------------------------------------------

TEST(MapGetTest, get_does_not_insert_on_miss) {
    map<std::string, int> m{{"alice", 42}};
    m.get("carol");
    EXPECT_EQ(m.size(), 1u);
    EXPECT_EQ(m.count("carol"), 0u);
}

// ---------------------------------------------------------------------------
// Monadic operations (value_or, and_then, transform)
// ---------------------------------------------------------------------------

TEST(MapGetTest, value_or_on_hit) {
    map<std::string, int> m{{"alice", 42}};
    EXPECT_EQ(m.get("alice").value_or(0), 42);
}

TEST(MapGetTest, value_or_on_miss) {
    map<std::string, int> m{{"alice", 42}};
    EXPECT_EQ(m.get("carol").value_or(-1), -1);
}

// ---------------------------------------------------------------------------
// Heterogeneous lookup (Compare::is_transparent via std::less<void>)
// ---------------------------------------------------------------------------

TEST(MapGetTest, heterogeneous_get_hit) {
    map<std::string, int, std::less<>> m{{"alice", 42}};
    // Look up using string_view — no std::string construction
    auto v = m.get(std::string_view{"alice"});
    ASSERT_TRUE(v.has_value());
    EXPECT_EQ(*v, 42);
}

TEST(MapGetTest, heterogeneous_get_miss) {
    map<std::string, int, std::less<>> m{{"alice", 42}};
    EXPECT_FALSE(m.get(std::string_view{"carol"}).has_value());
}

TEST(MapGetTest, heterogeneous_get_const) {
    const map<std::string, int, std::less<>> m{{"alice", 42}};
    auto                                     v = m.get(std::string_view{"alice"});
    ASSERT_TRUE(v.has_value());
    static_assert(std::is_same_v<decltype(v), beman::optional::optional<const int&>>);
}

// ---------------------------------------------------------------------------
// Works correctly across multiple entries
// ---------------------------------------------------------------------------

TEST(MapGetTest, multiple_entries_correct_lookup) {
    map<int, std::string> m{{1, "one"}, {2, "two"}, {3, "three"}};
    EXPECT_EQ(*m.get(1), "one");
    EXPECT_EQ(*m.get(2), "two");
    EXPECT_EQ(*m.get(3), "three");
    EXPECT_FALSE(m.get(4).has_value());
}

// ---------------------------------------------------------------------------
// Empty map
// ---------------------------------------------------------------------------

TEST(MapGetTest, get_on_empty_map_returns_nullopt) {
    map<int, int> m;
    EXPECT_FALSE(m.get(0).has_value());
}

} // namespace
