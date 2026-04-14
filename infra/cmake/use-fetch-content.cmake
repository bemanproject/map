cmake_minimum_required(VERSION 3.24)

include(FetchContent)

if(NOT BEMAN_MAP_LOCKFILE)
    set(BEMAN_MAP_LOCKFILE
        "lockfile.json"
        CACHE FILEPATH
        "Path to the dependency lockfile for beman.map."
    )
endif()

set(BemanMap_projectDir "${CMAKE_CURRENT_LIST_DIR}/../..")
message(TRACE "BemanMap_projectDir=\"${BemanMap_projectDir}\"")

message(TRACE "BEMAN_MAP_LOCKFILE=\"${BEMAN_MAP_LOCKFILE}\"")
file(
    REAL_PATH
    "${BEMAN_MAP_LOCKFILE}"
    BemanMap_lockfile
    BASE_DIRECTORY "${BemanMap_projectDir}"
    EXPAND_TILDE
)
message(DEBUG "Using lockfile: \"${BemanMap_lockfile}\"")

# Force CMake to reconfigure the project if the lockfile changes
set_property(
    DIRECTORY "${BemanMap_projectDir}"
    APPEND
    PROPERTY CMAKE_CONFIGURE_DEPENDS "${BemanMap_lockfile}"
)

# For more on the protocol for this function, see:
# https://cmake.org/cmake/help/latest/command/cmake_language.html#provider-commands
function(BemanMap_provideDependency method package_name)
    # Read the lockfile
    file(READ "${BemanMap_lockfile}" BemanMap_rootObj)

    # Get the "dependencies" field
    string(
        JSON
        BemanMap_dependenciesObj
        ERROR_VARIABLE BemanMap_error
        GET "${BemanMap_rootObj}"
        "dependencies"
    )
    if(BemanMap_error)
        message(FATAL_ERROR "${BemanMap_lockfile}: ${BemanMap_error}")
    endif()

    string(
        JSON
        BemanMap_numDependencies
        ERROR_VARIABLE BemanMap_error
        LENGTH "${BemanMap_dependenciesObj}"
    )
    if(BemanMap_error)
        message(FATAL_ERROR "${BemanMap_lockfile}: ${BemanMap_error}")
    endif()

    if(BemanMap_numDependencies EQUAL 0)
        return()
    endif()

    math(EXPR BemanMap_maxIndex "${BemanMap_numDependencies} - 1")
    foreach(BemanMap_index RANGE "${BemanMap_maxIndex}")
        set(BemanMap_errorPrefix "${BemanMap_lockfile}, dependency ${BemanMap_index}")

        string(JSON BemanMap_depObj ERROR_VARIABLE BemanMap_error GET "${BemanMap_dependenciesObj}" "${BemanMap_index}")
        if(BemanMap_error)
            message(FATAL_ERROR "${BemanMap_errorPrefix}: ${BemanMap_error}")
        endif()

        string(JSON BemanMap_name ERROR_VARIABLE BemanMap_error GET "${BemanMap_depObj}" "name")
        if(BemanMap_error)
            message(FATAL_ERROR "${BemanMap_errorPrefix}: ${BemanMap_error}")
        endif()

        string(JSON BemanMap_pkgName ERROR_VARIABLE BemanMap_error GET "${BemanMap_depObj}" "package_name")
        if(BemanMap_error)
            message(FATAL_ERROR "${BemanMap_errorPrefix}: ${BemanMap_error}")
        endif()

        string(JSON BemanMap_repo ERROR_VARIABLE BemanMap_error GET "${BemanMap_depObj}" "git_repository")
        if(BemanMap_error)
            message(FATAL_ERROR "${BemanMap_errorPrefix}: ${BemanMap_error}")
        endif()

        string(JSON BemanMap_tag ERROR_VARIABLE BemanMap_error GET "${BemanMap_depObj}" "git_tag")
        if(BemanMap_error)
            message(FATAL_ERROR "${BemanMap_errorPrefix}: ${BemanMap_error}")
        endif()

        if(method STREQUAL "FIND_PACKAGE")
            if(package_name STREQUAL BemanMap_pkgName)
                message(
                    DEBUG
                    "Redirecting find_package(${BemanMap_pkgName}) to FetchContent from ${BemanMap_repo}@${BemanMap_tag}"
                )
                FetchContent_Declare(
                    "${BemanMap_name}"
                    GIT_REPOSITORY "${BemanMap_repo}"
                    GIT_TAG "${BemanMap_tag}"
                    EXCLUDE_FROM_ALL
                )
                set(INSTALL_GTEST OFF)
                FetchContent_MakeAvailable("${BemanMap_name}")
                set("${BemanMap_pkgName}_FOUND" TRUE PARENT_SCOPE)
            endif()
        endif()
    endforeach()
endfunction()

cmake_language(
    SET_DEPENDENCY_PROVIDER BemanMap_provideDependency
    SUPPORTED_METHODS FIND_PACKAGE
)

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_LIST_DIR}")
