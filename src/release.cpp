#include <spdlog/spdlog.h>
#include <sstream>
#include <cctype>
#include <regex>
#include <format>
#include <iomanip>

#include "aptrepo/reference.hpp"
#include "aptrepo/internal/utils.hpp"
#include "aptrepo/internal/downloads.hpp"

#include "aptrepo/release.hpp"

aptrepo::Release::Release(aptrepo::internal::Download download)
    : m_flat(false)
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

    {
        auto it = m_fields.find("Architectures");
        if (it != m_fields.end())
        {
            std::string archs = it->second;
            std::stringstream ss(archs);
            std::string arch;
            while (std::getline(ss, arch, ' '))
            {
                if (!arch.empty())
                {
                    m_architectures.push_back(arch);
                }
            }
        }
        else
        {
            spdlog::warn("Release: Architectures field not found in release file.");
        }
    }

    {
        auto it = m_fields.find("Components");
        if (it != m_fields.end())
        {
            std::string comps = it->second;
            std::stringstream ss(comps);
            std::string comp;
            while (std::getline(ss, comp, ' '))
            {
                if (!comp.empty())
                {
                    m_components.push_back(comp);
                }
            }
        }
        else
        {
            spdlog::warn("Release: Components field not found in release file.");
        }
    }

    {
        auto it = m_fields.find("Date");
        if (it != m_fields.end())
        {
            tm tm = {};
            auto stream = std::istringstream(it->second);
            stream >> std::get_time(&tm, "%a, %d %b %Y %H:%M:%S");
            if (stream.fail())
            {
                spdlog::error("Release: Failed to parse date: {}", it->second);
            }
            else
            {
                auto time = std::mktime(&tm) - timezone;
                if (time == -1)
                {
                    spdlog::error("Release: Failed to parse date: {}", it->second);
                }
                else
                {
                    m_date = std::chrono::time_point<std::chrono::utc_clock, std::chrono::seconds>(std::chrono::seconds(time));
                }
            }
        }
        else
        {
            spdlog::warn("Release: Date field not found in release file.");
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
    m_fields[key] = value;
}

void aptrepo::Release::add_reference(std::string path, std::size_t size, std::string algorithm, std::string hash)
{
    if (auto search = m_references.find(path); search != m_references.end())
    {
        m_references[path]->add_hash(algorithm, hash);
    }
    else
    {
        auto ref = std::shared_ptr<aptrepo::Reference>{new aptrepo::Reference(m_base_url, path, size)};
        ref->add_hash(algorithm, hash);
        m_references[path] = ref;
    }
}

std::string aptrepo::Release::get_origin() const
{
    auto it = m_fields.find("Origin");
    if (it != m_fields.end())
    {
        return it->second;
    }
    else
    {
        spdlog::warn("Release: Origin field not found in release file.");
    }
    return {};
}

std::string aptrepo::Release::get_label() const
{
    auto it = m_fields.find("Label");
    if (it != m_fields.end())
    {
        return it->second;
    }
    return {};
}

std::string aptrepo::Release::get_suite() const
{
    auto it = m_fields.find("Suite");
    if (it != m_fields.end())
    {
        return it->second;
    }
    else
    {
        spdlog::warn("Release: Suite field not found in release file.");
    }
    return {};
}

std::string aptrepo::Release::get_version() const
{
    auto it = m_fields.find("Version");
    if (it != m_fields.end())
    {
        return it->second;
    }
    else
    {
        spdlog::warn("Release: Version field not found in release file.");
    }
    return {};
}

std::string aptrepo::Release::get_codename() const
{
    auto it = m_fields.find("Codename");
    if (it != m_fields.end())
    {
        return it->second;
    }
    else
    {
        spdlog::warn("Release: Codename field not found in release file.");
    }
    return {};
}

std::chrono::time_point<std::chrono::utc_clock, std::chrono::seconds> aptrepo::Release::get_date() const
{
    return m_date;
}

std::vector<std::string> aptrepo::Release::get_architectures() const
{
    return m_architectures;
}

std::vector<std::string> aptrepo::Release::get_components() const
{
    return m_components;
}

std::string aptrepo::Release::get_description() const
{
    auto it = m_fields.find("Description");
    if (it != m_fields.end())
    {
        return it->second;
    }
    else
    {
        spdlog::warn("Release: Description field not found in release file.");
    }
    return {};
}

std::string aptrepo::Release::get_url() const
{
    return m_url;
}

std::string aptrepo::Release::get_etag() const
{
    return m_etag;
}

std::string aptrepo::Release::get_base_url() const
{
    return m_base_url;
}

bool aptrepo::Release::is_flat() const
{
    return m_flat;
}

void aptrepo::Release::set_flat(bool flat)
{
    m_flat = flat;
}

std::vector<aptrepo::Reference> aptrepo::Release::get_references() const
{
    std::vector<aptrepo::Reference> refs;
    for (const auto &ref : m_references)
    {
        refs.push_back(*ref.second);
    }
    return refs;
}

std::vector<aptrepo::Reference> aptrepo::Release::get_references(std::string arch, std::string comp) const
{
    std::vector<aptrepo::Reference> refs;
    for (const auto &ref : m_references)
    {
        if (ref.second->get_architecture() == arch && ref.second->get_component() == comp)
        {
            refs.push_back(*ref.second);
        }
    }
    return refs;
}

std::vector<aptrepo::Reference> aptrepo::Release::get_references_for_comp(std::string comp) const
{
    std::vector<aptrepo::Reference> refs;
    for (const auto &ref : m_references)
    {
        if (ref.second->get_component() == comp)
        {
            refs.push_back(*ref.second);
        }
    }
    return refs;
}

std::vector<aptrepo::Reference> aptrepo::Release::get_references_for_arch(std::string arch) const
{
    std::vector<aptrepo::Reference> refs;
    for (const auto &ref : m_references)
    {
        if (ref.second->get_architecture() == arch)
        {
            refs.push_back(*ref.second);
        }
    }
    return refs;
}
