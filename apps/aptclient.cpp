#include <iostream>
#include <vector>
#include <tuple>
#include <string>

#include <spdlog/spdlog.h>
#include <cxxopts.hpp>
#include <cpr/cpr.h>

#include "aptrepo/version.hpp"
#include "aptrepo/aptrepo.hpp"

void setup_logging(bool debug)
{
    if (debug)
    {
        spdlog::set_level(spdlog::level::debug);
        spdlog::debug("Debug mode enabled.");
    }
    else
    {
        spdlog::set_level(spdlog::level::info);
    }
}

int main(int argc, char *argv[])
{
    cxxopts::Options options(argv[0], "Parse Debian APT repositories independent of the local systems package sources.");
    options.add_options()("debug", "Enable debugging")("h,help", "Print usage")("v,version", "Print version");

#if __aarch64__
    auto default_arch = "aarch64";
#else
    auto default_arch = "x86_64";
#endif

    options.add_options()("d,distro", "Distro name", cxxopts::value<std::string>()->default_value("none"));
    options.add_options()("a,arch", "CPU architecture", cxxopts::value<std::string>()->default_value(default_arch));

    options.parse_positional({"script", "server", "filenames"});

    auto result = options.parse(argc, argv);

    if (result.count("help"))
    {
        std::cout << options.help() << std::endl;
        exit(0);
    }

    if (result.count("version"))
    {
        std::cout << PROJECT_VERSION << std::endl;
        exit(0);
    }

    bool debug = result["debug"].as<bool>();

    setup_logging(debug);

    spdlog::info("AptRepo Version: {}", PROJECT_VERSION);

    cpr::Response r = cpr::Get(cpr::Url{"https://archive.ubuntu.com/ubuntu/dists/noble/InRelease"});
    spdlog::info("Response status code: {}", r.status_code);
    if (r.status_code != 200)
    {
        spdlog::error("Failed to fetch InRelease file. Status code: {}", r.status_code);
        return 1;
    }
    else
    {
        spdlog::info("Successfully fetched InRelease file:\n{}", r.text);
    }

    return 0;
}
