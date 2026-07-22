# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

cmake_minimum_required(VERSION 3.28...4.3)

project(
    beman.map
    DESCRIPTION "Better map lookups using optional<T&> (P3091)"
    LANGUAGES CXX
    VERSION 0.1.0
)

# [CMAKE.SKIP_TESTS]
option(
    BEMAN_MAP_BUILD_TESTS
    "Enable building tests and test infrastructure. Default: ${PROJECT_IS_TOP_LEVEL}. Values: { ON, OFF }."
    ${PROJECT_IS_TOP_LEVEL}
)

# [CMAKE.SKIP_EXAMPLES]
option(
    BEMAN_MAP_BUILD_EXAMPLES
    "Enable building examples. Default: ${PROJECT_IS_TOP_LEVEL}. Values: { ON, OFF }."
    ${PROJECT_IS_TOP_LEVEL}
)

# For find of beman-install-library
include(infra/cmake/beman-install-library.cmake)

find_package(beman.optional REQUIRED)

add_library(beman.map INTERFACE)
add_library(beman::map ALIAS beman.map)

# When beman.optional is brought in via FetchContent it provides the
# beman.optional target. When installed and found via find_package it also
# exports beman::optional. Ensure the alias always exists.
if(TARGET beman.optional AND NOT TARGET beman::optional)
    add_library(beman::optional ALIAS beman.optional)
endif()

target_sources(
    beman.map
    PUBLIC FILE_SET HEADERS BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(beman.map INTERFACE beman::optional)

set_target_properties(
    beman.map
    PROPERTIES VERIFY_INTERFACE_HEADER_SETS ${PROJECT_IS_TOP_LEVEL}
)

add_subdirectory(include/beman/map)

beman_install_library(
    beman.map
    TARGETS beman.map
    DEPENDENCIES "beman.optional"
)

if(BEMAN_MAP_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests/beman/map)
endif()

if(BEMAN_MAP_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
