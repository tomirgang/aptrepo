/******************************************************************************
 * @file downloads.hpp
 * @brief Header file for aptrepo download helper functions.
 *
 * These functions are intended for aptrepo internal usage and building
 * on top of the cpr library and curl.
 ******************************************************************************/

#pragma once

#include <string>

namespace aptrepo
{
    namespace internal
    {

        /******************************************************************************
         * Download class to encapsulate the URL, etag, and content of a
         * downloaded resource.
         ******************************************************************************/
        class Download
        {
        public:
            /******************************************************************************
             * Constructor for Download class.
             *
             * @param url URL of the downloaded resource.
             * @param etag ETag of the downloaded resource.
             * @param content Content of the downloaded resource as a string.
             ******************************************************************************/
            explicit Download(std::string url, std::string etag, std::string content)
                : m_url(std::move(url)), m_etag(std::move(etag)), m_content(std::move(content)) {};

            /******************************************************************************
             * Get the URL of the downloaded resource.
             *
             * @return URL as a string.
             ******************************************************************************/
            std::string get_url() const;

            /******************************************************************************
             * Get the ETag of the downloaded resource.
             *
             * @return ETag as a string.
             ******************************************************************************/
            std::string get_etag() const;

            /******************************************************************************
             * Get the content of the downloaded resource.
             *
             * @return Content as a string.
             ******************************************************************************/
            std::string get_content() const;

        private:
            std::string m_url;
            std::string m_etag;
            std::string m_content;
        };

        /******************************************************************************
         * Download the contents of a URL as std::string.
         *
         * This function is intended for internal use.
         *
         * @param url URL to download.
         * @return Download object containing the URL, etag, and content.
         ******************************************************************************/
        aptrepo::internal::Download download(std::string url);

        /******************************************************************************
         * Checks if the given URL was updated, using the provided etag.
         *
         * This function is intended for internal use.
         *
         * @param url URL to test and etag of current version.
         * @return true if the URL was updated, false otherwise.
         ******************************************************************************/
        bool needs_update(std::string url, std::string etag);
    }
}
