#include <spdlog/spdlog.h>
#include <sstream>
#include <cctype>
#include <regex>
#include <format>

#include "aptrepo/reference.hpp"
#include "aptrepo/internal/utils.hpp"
#include "aptrepo/internal/downloads.hpp"

#include "aptrepo/release.hpp"

aptrepo::Release::Release(aptrepo::internal::Download download)
{
    m_url = download.get_url();
    m_etag = download.get_etag();
    m_base_url = m_url.substr(0, m_url.find_last_of('/'));

    std::stringstream content(download.get_content());
    std::string line;

    std::string key;

    auto reference_regex = std::regex(R"(^\s+(\S+)\s+(\d+)\s+(\S+).*$)");

    while (std::getline(content, line))
    {
        if (line.empty() || line[0] == '#')
        {
            // Skip empty lines and comments
            continue;
        }

        if (line.contains("BEGIN PGP SIGNATURE"))
        {
            // Stop parsing at the PGP signature block
            break;
        }

        if (line.starts_with("----"))
        {
            // Skip PGP signature lines
            continue;
        }

        auto first_char = line[0];
        if ((first_char >= 'A' && first_char <= 'Z') || (first_char >= 'a' && first_char <= 'z'))
        {
            auto pos = line.find(':');
            if (pos != std::string::npos)
            {
                key = line.substr(0, pos);
                auto value = aptrepo::internal::trim(line.substr(pos + 1));
                if (!key.empty() && !value.empty())
                {
                    add_field(key, value);
                }
            }
        }
        else
        {
            auto match = std::regex_match(line, reference_regex);
            if (match)
            {
                std::smatch match_groups;
                std::regex_search(line, match_groups, reference_regex);
                std::string path = match_groups[3].str();
                std::size_t size = std::stoull(match_groups[2].str());
                std::string hash = match_groups[1].str();
                add_reference(path, size, key, hash);
            }
        }
    }
}

aptrepo::Release::operator std::string() const
{
    std::string result = "Release<" + m_url + ">\n";
    result += "Etag: " + m_etag + "\n";
    result += "Base URL: " + m_base_url + "\n";

    for (const auto &field : m_fields)
    {
        result += field.first + ": " + field.second + "\n";
    }

    for (const auto &ref : m_references)
    {
        result += ref.second->operator std::string() + "\n";
    }

    return result;
}

void aptrepo::Release::add_field(std::string key, std::string value)
{
    spdlog::debug("Adding field to Release: key: {}, value: {}", key, value);
    m_fields[key] = value;
}

void aptrepo::Release::add_reference(std::string path, std::size_t size, std::string algorithm, std::string hash)
{
    if (auto search = m_references.find(path); search != m_references.end())
    {
        spdlog::debug("Adding hash to existing reference: path: {}, algorithm: {}, hash: {}", path, algorithm, hash);
        m_references[path]->add_hash(algorithm, hash);
    }
    else
    {
        spdlog::debug("Creating new reference: path: {}, size: {}, algorithm: {}, hash: {}", path, size, algorithm, hash);
        auto ref = std::shared_ptr<aptrepo::Reference>{new aptrepo::Reference(m_base_url, path, size)};
        ref->add_hash(algorithm, hash);
        m_references[path] = ref;
    }
}
