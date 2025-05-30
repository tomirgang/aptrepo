/******************************************************************************
 * @file utils.hpp
 * @brief Header file for aptrepo internal utility functions.
 ******************************************************************************/

#pragma once

#include <string>

namespace aptrepo
{
    namespace internal
    {
        /******************************************************************************
         * Trim whitespace from the beginning and end of a string.
         *
         * @param source The string to be trimmed.
         * @return A new string with leading and trailing whitespace removed.
         ******************************************************************************/
        std::string trim(const std::string &source);
    }
}
