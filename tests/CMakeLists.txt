# Testing library
FetchContent_Declare(
  catch
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v2.13.6)
FetchContent_MakeAvailable(catch)
# Adds Catch2::Catch2

# Tests need to be added as executables first
add_executable(testlib testlib.cpp)
target_link_libraries(testlib PRIVATE project_core Catch2::Catch2)

add_test(NAME testlibtest COMMAND testlib)
