/******************************************************************************
 * @file release.hpp
 * @brief Header file for aptrepo::Release.
 *
 * A aptrepo::Release represents a parsed APT release file, including its
 * URL, ETag, base URL, fields, and references to files in the repository.
 * This class is used to encapsulate the information contained in a Release file
 * and provides methods to add fields and references.
 ******************************************************************************/

#pragma once

#include <string>
#include <map>
#include <cstddef>
#include <memory>

#include "aptrepo/reference.hpp"
#include "aptrepo/internal/downloads.hpp"

namespace aptrepo
{
    /******************************************************************************
     * Release class to encapsulate a parsed APT release file.
     *
     * This class includes the URL, ETag, content fields, and references
     * to files in the repository.
     ******************************************************************************/
    class Release
    {
    public:
        /******************************************************************************
         * Constructor for Release class.
         *
         * @param download Download object containing the URL, ETag, and content
         *                 of the release file.
         ******************************************************************************/
        explicit Release(aptrepo::internal::Download download);

        /******************************************************************************
         * Add a field to the Release.
         *
         * @param key   The key of the field.
         * @param value The value of the field.
         ******************************************************************************/
        void add_field(std::string key, std::string value);

        /******************************************************************************
         * Add a reference to a file in the Release.
         *
         * @param path       The path to the referenced file.
         * @param size       The size of the referenced file in bytes.
         * @param algorithm  The hash algorithm used (e.g., "sha256").
         * @param hash       The hash value of the file.
         ******************************************************************************/
        void add_reference(std::string path, std::size_t size, std::string algorithm, std::string hash);

        /******************************************************************************
         * Convert the Release to a string representation.
         *
         * @return String representation of the Release.
         ******************************************************************************/
        operator std::string() const;

    private:
        std::string m_url;
        std::string m_etag;
        std::string m_base_url;
        std::map<std::string, std::string> m_fields;
        std::map<std::string, std::shared_ptr<aptrepo::Reference>> m_references;
    };
}
