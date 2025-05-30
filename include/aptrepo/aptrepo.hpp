/******************************************************************************
 * @file aptrepo.hpp
 * @brief Header file for aptrepo library functions.
 *
 * This file describes the public interface of the aptrepo library.
 ******************************************************************************/

#pragma once

#include <string>
#include <map>
#include <cstddef>
#include <memory>

#include "aptrepo/release.hpp"

namespace aptrepo
{
    /******************************************************************************
     * The parse_release function is used to parse a Release file from a given URL.
     *
     * @param url The URL of the Release file to be parsed.
     * @return A Release object containing the parsed information.
     ******************************************************************************/
    Release parse_release(std::string url);
}
