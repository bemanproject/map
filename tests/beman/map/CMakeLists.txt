# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

find_package(GTest REQUIRED)

include(CheckCXXSourceCompiles)

# Detect whether <flat_map> is available in the current toolchain.
check_cxx_source_compiles(
    "#include <flat_map>
     int main() { std::flat_map<int,int> m; (void)m; return 0; }"
    BEMAN_MAP_HAS_FLAT_MAP
)

# map tests
add_executable(beman.map.tests.map)
target_sources(beman.map.tests.map PRIVATE map.test.cpp)
target_link_libraries(beman.map.tests.map PRIVATE beman::map GTest::gtest_main)

# unordered_map tests
add_executable(beman.map.tests.unordered_map)
target_sources(beman.map.tests.unordered_map PRIVATE unordered_map.test.cpp)
target_link_libraries(
    beman.map.tests.unordered_map
    PRIVATE beman::map GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(beman.map.tests.map)
gtest_discover_tests(beman.map.tests.unordered_map)

# flat_map tests — only built when <flat_map> is available (GCC 14+, Clang 19+, MSVC 2022+)
if(BEMAN_MAP_HAS_FLAT_MAP)
    add_executable(beman.map.tests.flat_map)
    target_sources(beman.map.tests.flat_map PRIVATE flat_map.test.cpp)
    target_link_libraries(
        beman.map.tests.flat_map
        PRIVATE beman::map GTest::gtest_main
    )
    gtest_discover_tests(beman.map.tests.flat_map)
else()
    message(
        WARNING
        "beman.map: <flat_map> not available — skipping flat_map tests"
    )
endif()
