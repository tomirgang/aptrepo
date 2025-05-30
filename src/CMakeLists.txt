set(HEADER_LIST
    "${PROJECT_SOURCE_DIR}/include/aptrepo/aptrepo.hpp"
    "${PROJECT_SOURCE_DIR}/include/aptrepo/internal/downloads.hpp"
    "${PROJECT_SOURCE_DIR}/include/aptrepo/reference.hpp"
    "${PROJECT_SOURCE_DIR}/include/aptrepo/release.hpp"
    "${PROJECT_SOURCE_DIR}/include/aptrepo/internal/utils.hpp")

add_library(aptrepo
            aptrepo.cpp
            downloads.cpp
            reference.cpp 
            release.cpp
            utils.cpp
            ${HEADER_LIST})

target_include_directories(aptrepo PUBLIC ../include)
target_link_libraries(aptrepo PRIVATE cpr::cpr spdlog::spdlog)

# IDEs should put the headers in a nice place
source_group(
  TREE "${PROJECT_SOURCE_DIR}/include"
  PREFIX "Header Files"
  FILES ${HEADER_LIST})
