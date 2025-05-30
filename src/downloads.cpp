#include <exception>

#include <spdlog/spdlog.h>
#include <cpr/cpr.h>

#include "aptrepo/internal/downloads.hpp"

std::string aptrepo::internal::Download::get_url() const
{
    return m_url;
}

std::string aptrepo::internal::Download::get_etag() const
{
    return m_etag;
}

std::string aptrepo::internal::Download::get_content() const
{
    return m_content;
}

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

aptrepo::internal::Download aptrepo::internal::download(std::string url)
{
    spdlog::info("Downloading from URL: {}", url);

    cpr::Response r = cpr::Get(cpr::Url{url});

    spdlog::debug("Response status code: {}", r.status_code);

    if (r.status_code != 200)
    {
        spdlog::error("Failed to download from URL: {}. Status code: {}", url, r.status_code);
        throw std::runtime_error("Download failed");
    }

    return Download(
        url,
        r.header["etag"],
        r.text);
}
