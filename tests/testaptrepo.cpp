
#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>

#include <cpr/cpr.h>
#include <spdlog/spdlog.h>

#include "aptrepo/internal/downloads.hpp"
#include "aptrepo/internal/utils.hpp"
#include "aptrepo/reference.hpp"
#include "aptrepo/release.hpp"
#include "aptrepo/aptrepo.hpp"

TEST_CASE("Check if update is needed", "[download][internal]")
{
    spdlog::set_level(spdlog::level::info);

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
    spdlog::set_level(spdlog::level::info);

    auto url = "http://archive.ubuntu.com/ubuntu/dists/noble/InRelease";

    auto content = aptrepo::internal::download(url).get_content();

    CHECK_THAT(content, Catch::Matchers::Contains("Origin: Ubuntu"));
    CHECK_THAT(content, Catch::Matchers::Contains("Suite: noble"));
}

TEST_CASE("Trim string", "[utils][internal]")
{
    spdlog::set_level(spdlog::level::info);

    auto result = aptrepo::internal::trim("  \n\r\t  Hello, World!  \n\r\t  ");

    CHECK_THAT(result, Catch::Matchers::Equals("Hello, World!"));
}

TEST_CASE("Reference", "[inrelease][data]")
{
    spdlog::set_level(spdlog::level::info);

    auto reference = aptrepo::Reference("http://archive.ubuntu.com/ubuntu/dists/noble", "main/binary-amd64/Packages.gz", 123456);
    reference.add_hash("sha256", "abc123");
    reference.add_hash("md5", "def456");
    std::string reference_str = reference;

    CHECK_THAT(reference_str, Catch::Matchers::Contains("http://archive.ubuntu.com/ubuntu/dists/noble/main/binary-amd64/Packages.gz"));
    CHECK_THAT(reference_str, Catch::Matchers::Contains("sha256=abc123"));
    CHECK_THAT(reference_str, Catch::Matchers::Contains("md5=def456"));

    CHECK_THAT(reference.get_architecture(), Catch::Matchers::Equals("amd64"));
    CHECK_THAT(reference.get_component(), Catch::Matchers::Equals("main"));

    auto reference2 = aptrepo::Reference("http://archive.ubuntu.com/ubuntu/dists/noble", "main/source/Sources.gz", 654321);
    CHECK_THAT(reference2.get_architecture(), Catch::Matchers::Equals("source"));
    CHECK_THAT(reference2.get_component(), Catch::Matchers::Equals("main"));

    auto reference3 = aptrepo::Reference("http://archive.ubuntu.com/ubuntu/dists/noble", "Contents-armhf.gz", 789012);
    CHECK_THAT(reference3.get_architecture(), Catch::Matchers::Equals("armhf"));
    CHECK_THAT(reference3.get_component(), Catch::Matchers::Equals(""));
}

TEST_CASE("Release", "[inrelease][data]")
{
    spdlog::set_level(spdlog::level::info);

    auto release_url = "http://archive.ubuntu.com/ubuntu/dists/noble/InRelease";
    auto etag = "3e76a-616ed332a1b79";
    auto content =
        "-----BEGIN PGP SIGNED MESSAGE-----\n\
Hash: SHA512\n\
\n\
Origin: Ubuntu\n\
Label: Ubuntu\n\
Suite: noble\n\
Version: 24.04\n\
Codename: noble\n\
Date: Thu, 25 Apr 2024 15:10:33 UTC\n\
Architectures: amd64 arm64 armhf i386 ppc64el riscv64 s390x\n\
Components: main restricted universe multiverse\n\
Description: Ubuntu Noble 24.04\n\
MD5Sum:\n\
 1ae40621b32609d6251d09b2a47ef936        829119597 Contents-amd64\n\
 2fc7d01e0a1c7b351738abcd571eec59         51301092 Contents-amd64.gz\n\
 a78c03f162892e93e91366e0ec2a4f13        826443945 Contents-arm64\n\
 c131a52c95ba1474f94558b33807c46c         51152650 Contents-arm64.gz\n\
 262e2d34f95ba40bbff222c1902a97de          7165069 main/binary-amd64/Packages\n\
SHA1:\n\
 daf36358068b71e8064ac4e1f9adb6ae765f72a6        829119597 Contents-amd64\n\
 61706fecb4e611d62e5630e51c177c7b7fc59d9b         51301092 Contents-amd64.gz\n\
 c1d9390c97a488b9ac6a82a643673066c853d2a3        826443945 Contents-arm64\n\
 babe10bac60f76c40c99e6991169dd4238b87ebb         51152650 Contents-arm64.gz\n\
 b81dce369f26057810bd379a3fbd85c8aef1a9ef          7165069 main/binary-amd64/Packages\n\
SHA256:\n\
 e945cdeadad8067c9b569e66c058f709d5aa4cd11d8099cc088dc192705e7bc7        829119597 Contents-amd64\n\
 c8718dbbacd1ab72675513cf0674ff9921fcf781d9f49c4c0eaf68a49c18adc1         51301092 Contents-amd64.gz\n\
 e277c84bdd3351fa7b71bb26d3d05d9df8a56a21df463e8ffa47c3adaa5d6366        826443945 Contents-arm64\n\
 c0c2eef334518bb29513157aed43bf0a064d69f4482f515e1d7268403c066620         51152650 Contents-arm64.gz\n\
 8f6f71ae839c8cba390a7643fcbbdacddb0bc7d12c1583a2dd80a1f8443a30e5          7165069 main/binary-amd64/Packages\n\
Acquire-By-Hash: yes\n\
-----BEGIN PGP SIGNATURE-----\n\
\n\
iQIzBAEBCgAdFiEE9uyzdiR07anSG3Aihxkg0ZkbyTwFAmYqcpkACgkQhxkg0Zkb\n\
yTy1ow//fX85IioBgUFZ720K233aS/WKYURW/orgOTDImg7bpz15WkfLltwJyxCj\n\
=Wb/6\n\
-----END PGP SIGNATURE-----";

    auto donwload = aptrepo::internal::Download(release_url, etag, content);
    auto release = aptrepo::Release(donwload);
    auto release_str = std::string(release);

    CHECK_THAT(release_str, Catch::Matchers::Contains("Release<http://archive.ubuntu.com/ubuntu/dists/noble/InRelease>"));
    CHECK_THAT(release_str, Catch::Matchers::Contains("Origin: Ubuntu"));
    CHECK_THAT(release_str, Catch::Matchers::Contains("Suite: noble"));
    CHECK_THAT(release_str, Catch::Matchers::Contains("Date: Thu, 25 Apr 2024 15:10:33 UTC"));
    CHECK_THAT(release_str, Catch::Matchers::Contains("Acquire-By-Hash: yes"));
    CHECK_THAT(release_str, Catch::Matchers::Contains("Reference<http://archive.ubuntu.com/ubuntu/dists/noble/Contents-amd64 829119597 MD5Sum=1ae40621b32609d6251d09b2a47ef936, SHA1=daf36358068b71e8064ac4e1f9adb6ae765f72a6, SHA256=e945cdeadad8067c9b569e66c058f709d5aa4cd11d8099cc088dc192705e7bc7>"));

    CHECK_THAT(release.get_origin(), Catch::Matchers::Equals("Ubuntu"));
    CHECK_THAT(release.get_label(), Catch::Matchers::Equals("Ubuntu"));
    CHECK_THAT(release.get_suite(), Catch::Matchers::Equals("noble"));
    CHECK_THAT(release.get_version(), Catch::Matchers::Equals("24.04"));
    CHECK_THAT(release.get_codename(), Catch::Matchers::Equals("noble"));
    REQUIRE(release.get_date().time_since_epoch() == std::chrono::seconds(1714057833));
    CHECK_THAT(release.get_architectures(), Catch::Matchers::UnorderedEquals(std::vector<std::string>{"amd64", "arm64", "armhf", "i386", "ppc64el", "riscv64", "s390x"}));
    CHECK_THAT(release.get_components(), Catch::Matchers::UnorderedEquals(std::vector<std::string>{"main", "restricted", "universe", "multiverse"}));
    CHECK_THAT(release.get_description(), Catch::Matchers::Equals("Ubuntu Noble 24.04"));

    REQUIRE(release.get_references().size() == 5);
    REQUIRE(release.get_references("amd64", "main").size() == 1);
    REQUIRE(release.get_references_for_comp("main").size() == 1);
    REQUIRE(release.get_references_for_arch("arm64").size() == 2);
}

TEST_CASE("parse_release", "[inrelease][api]")
{
    spdlog::set_level(spdlog::level::info);

    auto release_url = "http://archive.ubuntu.com/ubuntu/dists/noble/InRelease";
    auto release = aptrepo::parse_release(release_url);
    auto release_str = std::string(release);

    CHECK_THAT(release_str, Catch::Matchers::Contains("Release<http://archive.ubuntu.com/ubuntu/dists/noble/InRelease>"));
    CHECK_THAT(release_str, Catch::Matchers::Contains("Origin: Ubuntu"));
    CHECK_THAT(release_str, Catch::Matchers::Contains("Suite: noble"));
}
