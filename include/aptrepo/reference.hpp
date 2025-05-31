/******************************************************************************
 * @file reference.hpp
 * @brief Header file for aptrepo::Reference.
 *
 * A aptrepo::Reference represents a reference to a file in an APT repository,
 * including its URL, size, and hashes.
 ******************************************************************************/

#pragma once

#include <string>
#include <map>
#include <cstddef>

namespace aptrepo
{
    /******************************************************************************
     * Reference class to encapsulate a reference to a file in an APT repository.
     *
     * This class includes the URL, size in bytes, and hashes of the
     * referenced file.
     ******************************************************************************/
    class Reference
    {
    public:
        /******************************************************************************
         * Constructor for Reference class.
         *
         * @param base_url Base URL to complete the path.
         * @param path Path to the referenced file.
         * @param size_bytes Size of the referenced file in bytes.
         ******************************************************************************/
        Reference(std::string base_url, std::string path, std::size_t size_bytes);

        /******************************************************************************
         * Add a hash for the referenced file.
         *
         * @param algorithm Hash algorithm (e.g., "sha256").
         * @param hash Hash value of the file.
         ******************************************************************************/
        void add_hash(std::string algorithm, std::string hash);

        /******************************************************************************
         * Convert the Reference to a string representation.
         *
         * @return String representation of the Reference.
         ******************************************************************************/
        operator std::string() const;

        /******************************************************************************
         * Get the architectures of the Reference.
         *
         * @return Architecture as a string.
         ******************************************************************************/
        std::string get_architecture() const;

        /******************************************************************************
         * Get the component of the Reference.
         *
         * @return Component as a string.
         ******************************************************************************/
        std::string get_component() const;

    private:
        std::string m_arch;
        std::string m_comp;
        std::string m_base_url;
        std::string m_path;
        std::size_t m_size_bytes;
        std::map<std::string, std::string> m_hashes;
    };
}
