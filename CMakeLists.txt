cmake_minimum_required(VERSION 4.0)

project(
    aptrepo
    VERSION 0.1.0
    DESCRIPTION "Aptrepo is a C++ library for interfacing with Debian APT repositories."
    LANGUAGES CXX)

# Only do these if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

    # Optionally set things like CMAKE_CXX_STANDARD, CMAKE_POSITION_INDEPENDENT_CODE here
    set(CMAKE_CXX_STANDARD 23 CACHE STRING "The C++ standard to use")
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)

    if (MSVC)
        # warning level 4
        add_compile_options(/W4)
    else()
        # additional warnings
        add_compile_options(-Wall -Wextra -Wpedantic)
    endif()

    # Find packages go here.

    configure_file (
      "${PROJECT_SOURCE_DIR}/include/aptrepo/version.hpp.in"
      "${PROJECT_BINARY_DIR}/include/aptrepo/version.hpp"
    )
    include_directories(${CMAKE_CURRENT_BINARY_DIR}/include)

    # Let's nicely support folders in IDEs
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)



    # Testing only available if this is the main app
    # Note this needs to be done in the main CMakeLists
    # since it calls enable_testing, which must be in the
    # main CMakeLists.
    set(BUILD_TESTING ON)
    include(CTest)

    # Docs only available if this is the main app
    find_package(Doxygen)
    if(Doxygen_FOUND)
        add_subdirectory(docs)
    else()
        message(STATUS "Doxygen not found, not building docs")
    endif()
else()
    message(WARNING "Aptrepo is being used as a submodule, not building the main project.")
endif()

# FetchContent added in CMake 3.11, downloads during the configure step
# FetchContent_MakeAvailable was added in CMake 3.14; simpler usage
include(FetchContent)

FetchContent_Declare(
  cpr
  GIT_REPOSITORY https://github.com/libcpr/cpr.git
  GIT_TAG 1.11.2)
FetchContent_MakeAvailable(cpr)

FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG v1.15.3)
FetchContent_MakeAvailable(spdlog)

# The compiled library code is here
add_subdirectory(src)

# The executable code is here
add_subdirectory(apps)

# Testing only available if this is the main app
# Emergency override MODERN_CMAKE_BUILD_TESTING provided as well
if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR MODERN_CMAKE_BUILD_TESTING) AND BUILD_TESTING)
    message(STATUS "Enabled testing for ${PROJECT_NAME}")
    add_subdirectory(tests)
else()
    message(WARNING "Testing not enabled for ${PROJECT_NAME}")
    if(NOT CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
        message(STATUS "Project name is not matching ${CMAKE_PROJECT_NAME} != ${PROJECT_NAME}). Aptrepo seems to be used as submodule.")
    endif()
endif()

# Inspired by https://gitlab.com/CLIUtils/modern-cmake
