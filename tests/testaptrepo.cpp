
#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>

#include <cpr/cpr.h>
#include <spdlog/spdlog.h>

#include <aptrepo/aptrepo.hpp>

TEST_CASE("Check if update is needed", "[download][internal]")
{
    spdlog::set_level(spdlog::level::debug);

    auto url = "http://archive.ubuntu.com/ubuntu/dists/noble/InRelease";

    cpr::Response r = cpr::Head(cpr::Url{url});

    REQUIRE(r.status_code == 200);

    std::string etag = r.header["etag"];

    spdlog::debug("URL: {}", url);
    spdlog::debug("ETag: {}", etag);
    spdlog::debug("Status-code: {}", r.status_code);

    REQUIRE(aptrepo::internal::needs_update(url, etag) == false);

    auto not_mathching_etag = "no-match-etag";
    REQUIRE(aptrepo::internal::needs_update(url, not_mathching_etag) == true);
}

TEST_CASE("Download URL content", "[download][internal]")
{
    spdlog::set_level(spdlog::level::debug);

    auto url = "http://archive.ubuntu.com/ubuntu/dists/noble/InRelease";

    auto content = aptrepo::internal::download(url);

    CHECK_THAT(content, Catch::Matchers::Contains("Origin: Ubuntu"));
    CHECK_THAT(content, Catch::Matchers::Contains("Suite: noble"));
}
