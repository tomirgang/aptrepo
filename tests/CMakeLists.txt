# Testing library
FetchContent_Declare(
  catch
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v2.13.6)
FetchContent_MakeAvailable(catch)
# Adds Catch2::Catch2

# Tests need to be added as executables first
add_executable(testlibaptrepo testlibaptrepo.cpp)
target_link_libraries(testlibaptrepo PRIVATE libaptrepo Catch2::Catch2)

add_test(NAME libaptrepotest COMMAND testlibaptrepo)
