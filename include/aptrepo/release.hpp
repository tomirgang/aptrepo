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
#include <chrono>
#include <vector>

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

        /******************************************************************************
         * Get the origin of the Release.
         *
         * @return Origin as a string.
         ******************************************************************************/
        std::string get_origin() const;

        /******************************************************************************
         * Get the label of the Release.
         *
         * @return Label as a string.
         ******************************************************************************/
        std::string get_label() const;

        /******************************************************************************
         * Get the suite of the Release.
         *
         * @return Suite as a string.
         ******************************************************************************/
        std::string get_suite() const;

        /******************************************************************************
         * Get the version of the Release.
         *
         * @return Version as a string.
         ******************************************************************************/
        std::string get_version() const;

        /******************************************************************************
         * Get the codename of the Release.
         *
         * @return Codename as a string.
         ******************************************************************************/
        std::string get_codename() const;

        /******************************************************************************
         * Get the date of the Release.
         *
         * @return Date as a time_point in UTC seconds.
         ******************************************************************************/
        std::chrono::time_point<std::chrono::utc_clock, std::chrono::seconds> get_date() const;

        /******************************************************************************
         * Get the architectures supported by the Release.
         *
         * * @return Vector of architecture strings.
         ******************************************************************************/
        std::vector<std::string> get_architectures() const;

        /******************************************************************************
         * Get the components of the Release.
         *
         * @return Vector of component strings.
         ******************************************************************************/
        std::vector<std::string> get_components() const;

        /******************************************************************************
         * Get the description of the Release.
         *
         * @return Description as a string.
         ******************************************************************************/
        std::string get_description() const;

        /******************************************************************************
         * Get the URL of the InRelease file.
         *
         * @return URL as a string.
         ******************************************************************************/
        std::string get_url() const;

        /******************************************************************************
         * Get the ETag of the Release.
         *
         * @return ETag as a string.
         ******************************************************************************/
        std::string get_etag() const;

        /******************************************************************************
         * Get the base URL of the Release.
         *
         * @return Base URL as a string.
         ******************************************************************************/
        std::string get_base_url() const;

        /******************************************************************************
         * Check if the repository is a flat repository.
         *
         * @return True if flat, false otherwise.
         ******************************************************************************/
        bool is_flat() const;

        /******************************************************************************
         * Set the repository as a flat repository.
         *
         * @param flat True to set as flat, false otherwise.
         ******************************************************************************/
        void set_flat(bool flat);

        /******************************************************************************
         * Get all references in the Release.
         *
         * @return Vector of aptrepo::Reference objects.
         ******************************************************************************/
        std::vector<aptrepo::Reference> get_references() const;

        /******************************************************************************
         * Get references for a specific architecture and component.
         *
         * @param arch  The architecture to filter by.
         * @param comp  The component to filter by.
         * @return Vector of aptrepo::Reference objects matching the criteria.
         ******************************************************************************/
        std::vector<aptrepo::Reference> get_references(std::string arch, std::string comp) const;

        /******************************************************************************
         * Get references for a specific component.
         *
         * @param comp  The component to filter by.
         * @return Vector of aptrepo::Reference objects for the specified component.
         ******************************************************************************/
        std::vector<aptrepo::Reference> get_references_for_comp(std::string comp) const;

        /******************************************************************************
         * Get references for a specific architecture.
         *
         * @param arch  The architecture to filter by.
         * @return Vector of aptrepo::Reference objects for the specified architecture.
         ******************************************************************************/
        std::vector<aptrepo::Reference> get_references_for_arch(std::string arch) const;

    private:
        bool m_flat;
        std::string m_url;
        std::string m_etag;
        std::string m_base_url;
        std::chrono::time_point<std::chrono::utc_clock, std::chrono::seconds> m_date;
        std::vector<std::string> m_architectures;
        std::vector<std::string> m_components;
        std::map<std::string, std::string> m_fields;
        std::map<std::string, std::shared_ptr<aptrepo::Reference>> m_references;
    };
}
