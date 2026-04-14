// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

// Demonstrates heterogeneous lookup with beman::map::map::get().
//
// When the comparator is std::less<> (i.e., has is_transparent), .get()
// accepts any key type comparable to the stored key — avoiding unnecessary
// conversions (e.g., no std::string constructed from a string_view).

#include <beman/map/map.hpp>
#include <beman/map/unordered_map.hpp>

#include <functional>
#include <iostream>
#include <string>
#include <string_view>

// Transparent hash for std::string / std::string_view interop.
struct string_hash {
    using is_transparent = void;
    std::size_t operator()(std::string_view sv) const noexcept { return std::hash<std::string_view>{}(sv); }
    std::size_t operator()(const std::string& s) const noexcept { return std::hash<std::string_view>{}(s); }
};

int main() {
    // --- map with transparent comparator ---
    beman::map::map<std::string, int, std::less<>> m{{"hello", 1}, {"world", 2}};

    std::string_view key = "hello";
    if (auto v = m.get(key)) { // no std::string construction
        std::cout << "map[\"hello\"] = " << *v << "\n"; // 1
    }

    // --- const heterogeneous lookup ---
    const auto& cm = m;
    auto        v2 = cm.get(std::string_view{"world"});
    std::cout << "const map[\"world\"] = " << v2.value_or(-1) << "\n"; // 2

    // --- unordered_map with transparent hash + equal ---
    beman::map::unordered_map<std::string, int, string_hash, std::equal_to<>> um;
    um.emplace("foo", 42);

    if (auto v = um.get(std::string_view{"foo"})) { // no std::string construction
        std::cout << "unordered_map[\"foo\"] = " << *v << "\n"; // 42
    }

    std::cout << "unordered_map[\"bar\"] = " << um.get(std::string_view{"bar"}).value_or(0) << "\n"; // 0

    return 0;
}
