# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

set(ALL_EXAMPLES basic_get heterogeneous_lookup)

foreach(example ${ALL_EXAMPLES})
    add_executable(beman.map.examples.${example})
    target_sources(beman.map.examples.${example} PRIVATE ${example}.cpp)
    target_link_libraries(
        beman.map.examples.${example}
        PRIVATE beman::map
    )
endforeach()
