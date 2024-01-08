#pragma once

#include <string>
#include <string.h>


namespace utils {

    /**
     * @brief Creates a new raw string from an existing instance of std::string.
     * @param str The std::string object to copy.
     * @return newly allocated raw string.
     */
    char * createRawString(std::string const& str);

}
