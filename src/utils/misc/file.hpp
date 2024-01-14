#pragma once

#include <vector>
#include <string>
#include <filesystem>


namespace utils {

    /**
     * @brief Check if a file exists.
     * @param path Path to the file to check.
     * @throw std::runtime_error if file/directory does not exist.
     */
    void checkExists(std::filesystem::path const& path);

    /**
     * @brief Check if a file is a regular file.
     * @param path Path to the file to check.
     * @throw std::runtime_error if file/directory is not a regular file.
     */
    void checkIsRegularFile(std::filesystem::path const& path);

    /**
     * @brief Load a file as a char array.
     * @param path Path to the file to load.
     * @return Vector of chars containing file content.
     */
    std::vector<char> readBinaryFile(std::filesystem::path const& path);

}
