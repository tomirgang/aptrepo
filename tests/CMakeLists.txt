# Testing library
FetchContent_Declare(
  catch
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v2.13.6)
FetchContent_MakeAvailable(catch)


# Tests need to be added as executables first
add_executable(testaptrepo testaptrepo.cpp)
target_link_libraries(testaptrepo PRIVATE aptrepo Catch2::Catch2 cpr::cpr spdlog::spdlog)

add_test(NAME aptrepotest COMMAND testaptrepo)
