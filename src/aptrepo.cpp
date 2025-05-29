#include <spdlog/spdlog.h>
#include <cpr/cpr.h>
#include <exception>

#include "aptrepo/aptrepo.hpp"

bool aptrepo::internal::needs_update(std::string url, std::string etag)
{
    cpr::Response r = cpr::Head(cpr::Url{url}, cpr::Header{{"If-None-Match", etag}});

    if (r.status_code == 304)
    {
        spdlog::info("No update needed for URL: {}", url);
        return false;
    }

    if (r.status_code == 200)
    {
        spdlog::info("Update available for URL: {}", url);
    }
    else
    {
        spdlog::error("Failed to check update for URL: {}. Status code: {}", url, r.status_code);
    }

    return true;
}

std::string aptrepo::internal::download(std::string url)
{
    spdlog::info("Downloading from URL: {}", url);

    cpr::Response r;
    for (auto i = 0; i < 3; i++)
    {
        r = cpr::Get(cpr::Url{url});
        if (r.status_code != 0)
        {
            break;
        }
        spdlog::debug("Attempt {}: Status code: {}", i + 1, r.status_code);
    }

    spdlog::debug("Response status code: {}", r.status_code);

    if (r.status_code != 200)
    {
        spdlog::error("Failed to download from URL: {}. Status code: {}", url, r.status_code);
        throw std::runtime_error("Download failed");
    }

    return std::string{r.text};
}
