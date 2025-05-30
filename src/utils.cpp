#include <spdlog/spdlog.h>
#include <cpr/cpr.h>
#include <exception>
#include <sstream>
#include <cctype>
#include <regex>
#include <format>

#include "aptrepo/internal/utils.hpp"

std::string aptrepo::internal::trim(const std::string &source)
{
    std::string s(source);
    s.erase(0, s.find_first_not_of(" \n\r\t"));
    s.erase(s.find_last_not_of(" \n\r\t") + 1);
    return s;
}
