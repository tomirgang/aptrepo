
set(DOXYGEN_EXTRACT_ALL YES)
set(DOXYGEN_BUILTIN_STL_SUPPORT YES)
doxygen_add_docs(docs project/lib.hpp "${CMAKE_CURRENT_SOURCE_DIR}/main.md"
                 WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/include")
