cmake_minimum_required(VERSION 3.15...4.0)

project(
    project
    VERSION 1.0.0.0
    DESCRIPTION "C++ CMake Template"
    LANGUAGES CXX)


# Only do these if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

    # Optionally set things like CMAKE_CXX_STANDARD, CMAKE_POSITION_INDEPENDENT_CODE here
    set(CMAKE_CXX_STANDARD 23 CACHE STRING "The C++ standard to use")
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)

    # Find packages go here.

    configure_file (
      "${PROJECT_SOURCE_DIR}/include/project/version.hpp.in"
      "${PROJECT_BINARY_DIR}/include/project/version.hpp"
    )
    include_directories(${CMAKE_CURRENT_BINARY_DIR}/include)

    # Let's nicely support folders in IDEs
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    # Testing only available if this is the main app
    # Note this needs to be done in the main CMakeLists
    # since it calls enable_testing, which must be in the
    # main CMakeLists.
    include(CTest)

    # Docs only available if this is the main app
    find_package(Doxygen)
    if(Doxygen_FOUND)
        add_subdirectory(docs)
    else()
        message(STATUS "Doxygen not found, not building docs")
    endif()
endif()

# FetchContent added in CMake 3.11, downloads during the configure step
# FetchContent_MakeAvailable was added in CMake 3.14; simpler usage
include(FetchContent)

# The compiled library code is here
add_subdirectory(src)

# The executable code is here
add_subdirectory(apps)

# Testing only available if this is the main app
# Emergency override MODERN_CMAKE_BUILD_TESTING provided as well
if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR MODERN_CMAKE_BUILD_TESTING)
   AND BUILD_TESTING)
    add_subdirectory(tests)
endif()

# Inspired by https://gitlab.com/CLIUtils/modern-cmake
