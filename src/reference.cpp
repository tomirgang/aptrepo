#include <format>

#include "spdlog/spdlog.h"

#include "aptrepo/reference.hpp"

aptrepo::Reference::Reference(std::string base_url, std::string path, std::size_t size_bytes)
    : m_base_url(std::move(base_url)), m_path(std::move(path)), m_size_bytes(size_bytes)
{
    spdlog::debug("Creating Reference with base_url: {}, path: {}, size_bytes: {}", m_base_url, m_path, m_size_bytes);

    if (m_path.starts_with("Contents"))
    {
        spdlog::debug("Path starts with 'Contents', extracting architecture");

        auto pos = m_path.find('-');
        if (pos != std::string::npos)
        {
            auto next_pos = m_path.find('.', pos + 1);
            if (next_pos != std::string::npos)
            {

                m_arch = m_path.substr(pos + 1, next_pos - pos - 1);
            }
            else
            {
                m_arch = m_path.substr(pos + 1);
            }
        }
        spdlog::debug("Extracted architecture: {}", m_arch);
    }
    else
    {
        auto pos = m_path.find('/');
        if (pos != std::string::npos)
        {
            spdlog::debug("Path contains '/', extracting component and architecture");

            auto next_pos = m_path.find('/', pos + 1);
            if (next_pos != std::string::npos)
            {
                auto folder = m_path.substr(pos + 1, next_pos - pos - 1);

                spdlog::debug("Extracted folder: {}", folder);

                if (folder == "source")
                {
                    m_arch = "source";
                }

                pos = folder.find('-', pos + 1);
                if (pos != std::string::npos)
                {
                    auto first = folder.substr(0, pos);
                    if (first == "binary")
                    {
                        m_arch = folder.substr(pos + 1);
                    }
                }
            }
        }
    }

    auto pos = m_path.find('/');
    if (pos != std::string::npos)
    {
        m_comp = m_path.substr(0, pos);
    }
}

void aptrepo::Reference::add_hash(std::string algorithm, std::string hash)
{
    m_hashes[algorithm] = hash;
}

aptrepo::Reference::operator std::string() const
{
    auto hashes_str = std::string{};
    for (const auto &hash : m_hashes)
    {
        if (!hashes_str.empty())
        {
            hashes_str += ", ";
        }
        hashes_str += hash.first + "=" + hash.second;
    }
    return std::format("Reference<{}/{} {} {}>", m_base_url, m_path, m_size_bytes, hashes_str);
}

std::string aptrepo::Reference::get_architecture() const
{
    return m_arch;
}

std::string aptrepo::Reference::get_component() const
{
    return m_comp;
}
