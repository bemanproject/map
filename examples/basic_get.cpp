// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

// Demonstrates basic usage of beman::map::map::get() from P3091.
//
// Compare the three lookup strategies for std::map:
//
//   operator[] — inserts a default value on miss (silent mutation!)
//   .at()      — throws std::out_of_range on miss
//   .get()     — returns optional<T&>; safe, non-throwing, non-inserting

#include <beman/map/map.hpp>

#include <iostream>
#include <string>

int main() {
    beman::map::map<std::string, int> scores{{"alice", 95}, {"bob", 82}};

    // --- hit: present key ---
    if (auto v = scores.get("alice")) {
        std::cout << "alice: " << *v << "\n"; // 95
    }

    // --- miss: absent key, no insertion, no exception ---
    auto carol = scores.get("carol");
    if (!carol.has_value()) {
        std::cout << "carol: not found\n";
    }
    std::cout << "map size after miss: " << scores.size() << "\n"; // still 2

    // --- value_or: concise default ---
    int dave_score = scores.get("dave").value_or(0);
    std::cout << "dave (default): " << dave_score << "\n"; // 0

    // --- mutation via the returned reference ---
    if (auto v = scores.get("bob")) {
        *v += 10; // bob gets a bonus
    }
    std::cout << "bob after bonus: " << *scores.get("bob") << "\n"; // 92

    return 0;
}
