set(HEADER_LIST "${PROJECT_SOURCE_DIR}/include/libaptrepo/libaptrepo.hpp")

add_library(libaptrepo libaptrepo.cpp ${HEADER_LIST})
target_include_directories(libaptrepo PUBLIC ../include)

# IDEs should put the headers in a nice place
source_group(
  TREE "${PROJECT_SOURCE_DIR}/include"
  PREFIX "Header Files"
  FILES ${HEADER_LIST})
