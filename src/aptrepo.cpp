#include <spdlog/spdlog.h>

#include "aptrepo/internal/downloads.hpp"
#include "aptrepo/release.hpp"

#include "aptrepo/aptrepo.hpp"

aptrepo::Release aptrepo::parse_release(std::string url)
{
    spdlog::info("Parsing release from URL: {}", url);

    auto dl = aptrepo::internal::download(url);

    return Release(dl);
}
