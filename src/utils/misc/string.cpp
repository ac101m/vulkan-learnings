#include "utils/misc/string.hpp"


namespace utils {

    char * createRawString(std::string const& str) {
        char * const chars = new char[str.length() + 1];
        strcpy(chars, str.c_str());
        return chars;
    }

}
