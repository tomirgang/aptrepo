set(HEADER_LIST "${project_SOURCE_DIR}/include/project/lib.hpp")

add_library(project_core lib.cpp ${HEADER_LIST})
target_include_directories(project_core PUBLIC ../include)

# IDEs should put the headers in a nice place
source_group(
  TREE "${PROJECT_SOURCE_DIR}/include"
  PREFIX "Header Files"
  FILES ${HEADER_LIST})
