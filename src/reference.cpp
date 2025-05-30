#include <format>

#include "aptrepo/reference.hpp"

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
