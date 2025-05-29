
FetchContent_Declare(
  cxxopts
  GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
  GIT_TAG v3.3.1)
FetchContent_MakeAvailable(cxxopts)

add_executable(aptclient aptclient.cpp)

target_link_libraries(aptclient PRIVATE aptrepo spdlog::spdlog cxxopts::cxxopts cpr::cpr)

