#pragma once

#include <string>

namespace aptrepo
{

    namespace internal
    {

        /******************************************************************************
         * Download the contents of a URL as std::string.
         *
         * This function is intended for internal use.
         *
         * @param url URL to download.
         ******************************************************************************/
        std::string download(std::string url);

        /******************************************************************************
         * Checks if the given URL was updated, using the provided etag.
         *
         * This function is intended for internal use.
         *
         * @param url URL to test and etag of current version.
         ******************************************************************************/
        bool needs_update(std::string url, std::string etag);

    }
}
