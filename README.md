# beman.map: Better Map Lookups for C++

<!--
SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
-->

<!-- markdownlint-disable line-length -->
[![Library Status](https://raw.githubusercontent.com/bemanproject/beman/refs/heads/main/images/badges/beman_badge-beman_library_under_development.svg)](https://github.com/bemanproject/beman/blob/main/docs/beman_library_maturity_model.md#the-beman-library-maturity-model)
![Continuous Integration Tests](https://github.com/bemanproject/map/actions/workflows/ci_tests.yml/badge.svg)
![Lint Check (pre-commit)](https://github.com/bemanproject/map/actions/workflows/pre-commit-check.yml/badge.svg)
![Standard Target](https://github.com/bemanproject/beman/blob/main/images/badges/cpp29.svg)
[![Compiler Explorer Example](https://img.shields.io/badge/Try%20it%20on%20Compiler%20Explorer-grey?logo=compilerexplorer&logoColor=67c52a)](https://godbolt.org/z/7MWhaP6a3)
<!-- markdownlint-restore -->

`beman.map` implements safer, non-throwing map lookups returning `optional<T&>` instead
of throwing (`.at()`) or silently inserting (`operator[]`).

**Implements**: [Better lookups for `map`, `unordered_map`, and `flat_map` (P3091R5)](https://wg21.link/P3091R5).

**Status**: [Under development and not yet ready for production use.](https://github.com/bemanproject/beman/blob/main/docs/beman_library_maturity_model.md#under-development-and-not-yet-ready-for-production-use)

## License

`beman.map` is licensed under the Apache License v2.0 with LLVM Exceptions.

## Usage

P3091 adds a `.get(key)` member function to `map`, `unordered_map`, and `flat_map`.
It returns `optional<mapped_type&>` a non-throwing, never inserting, and composable
with all `optional` monadic operations.

```cpp
#include <beman/map/map.hpp>

beman::map::map<std::string, int> scores{{"alice", 42}, {"bob", 7}};

// Safe lookup: returns optional<int&>
if (auto v = scores.get("alice")) {
    std::cout << *v << "\n";  // 42
}

// Missing key: returns nullopt, no exception, no insertion
auto v = scores.get("carol");
assert(!v.has_value());

// Monadic composition
int result = scores.get("alice").value_or(0);  // 42
```

### Heterogeneous lookup

When the comparator has `is_transparent` (e.g. `std::less<void>`), heterogeneous
key types work without constructing a `key_type`:

```cpp
beman::map::map<std::string, int, std::less<>> m{{"hello", 1}};
auto v = m.get(std::string_view{"hello"});  // optional<int&>
```

Full runnable examples can be found in [`examples/`](examples/).

## Dependencies

### Build Environment

This project requires at least the following to build:

* A C++ compiler that conforms to the C++23 standard or greater
* CMake 3.30 or later
* (Test Only) GoogleTest
* [beman.optional](https://github.com/bemanproject/optional) (auto-fetched via lockfile)
* (Test only) GoogleTest (auto-fetched via lockfile)

You can disable building tests by setting CMake option `BEMAN_MAP_BUILD_TESTS` to
`OFF` when configuring the project.

You can disable building examples by setting CMake option `BEMAN_MAP_BUILD_EXAMPLES` to
`OFF` when configuring the project.

### Supported Platforms

| Compiler   | Version | C++ Standards | Standard Library  |
|------------|---------|---------------|-------------------|
| GCC        | 15-14   | C++23         | libstdc++         |
| Clang      | 22-19   | C++23         | libstdc++, libc++ |
| AppleClang | latest  | C++23         | libc++            |
| MSVC       | latest  | C++23         | MSVC STL          |

## Development

See the [Contributing Guidelines](CONTRIBUTING.md).

### Configure, Build, Test and Run locally

```bash
# 1. Configure
cmake -S . -B build/debug \
  -G Ninja \
  -DCMAKE_CXX_STANDARD=23 \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_PROJECT_TOP_LEVEL_INCLUDES=./infra/cmake/use-fetch-content.cmake

# 2. Build
cmake --build build/debug

# 3. Test
ctest --test-dir build/debug --output-on-failure

# 4. Run examples
./build/debug/examples/beman.map.examples.basic_get
./build/debug/examples/beman.map.examples.heterogeneous_lookup
```

Alternatively, use a CMake workflow preset (requires CMake 3.30+):

```bash
cmake --workflow --preset gcc-debug    # debug build
cmake --workflow --preset gcc-release  # release build
```

## Integrate beman.map into your project

### Build and Install

```bash
cmake --workflow --preset gcc-release
sudo cmake --install build/gcc-release
```

### CMake Configuration

```cmake
find_package(beman.map REQUIRED)
target_link_libraries(yourlib PUBLIC beman::map)
```

### Using beman.map

```cpp
#include <beman/map/map.hpp>            // beman::map::map
#include <beman/map/unordered_map.hpp>  // beman::map::unordered_map
#include <beman/map/flat_map.hpp>       // beman::map::flat_map
```
