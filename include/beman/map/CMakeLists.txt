# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

include(CheckCXXSourceCompiles)

check_cxx_source_compiles(
    "#include <flat_map>
     int main() { std::flat_map<int,int> m; (void)m; return 0; }"
    BEMAN_MAP_HAS_FLAT_MAP
)

set(_beman_map_headers map.hpp unordered_map.hpp)
if(BEMAN_MAP_HAS_FLAT_MAP)
    list(APPEND _beman_map_headers flat_map.hpp)
else()
    message(
        WARNING
        "beman.map: <flat_map> not available — flat_map.hpp not installed"
    )
endif()

target_sources(beman.map PUBLIC FILE_SET HEADERS FILES ${_beman_map_headers})
