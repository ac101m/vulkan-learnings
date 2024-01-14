#include "utils/misc/file.hpp"

#include <fstream>



namespace utils {

    void checkExists(std::filesystem::path const& path) {
        if (!std::filesystem::exists(path)) {
            throw std::runtime_error("No such file or directory '" + std::string(path) + "'");
        }
    }


    void checkIsRegularFile(std::filesystem::path const& path) {
        if (!std::filesystem::is_regular_file(path)) {
            throw std::runtime_error("'" + std::string(path) + "' is not a regular file.");
        }
    }


    std::vector<char> readBinaryFile(std::filesystem::path const& path) {
        checkExists(path);
        checkIsRegularFile(path);

        std::ifstream ifs(path, std::ios::ate | std::ios::binary);

        size_t const fileSize = (size_t) ifs.tellg();
        std::vector<char> buffer(fileSize);

        ifs.seekg(0);
        ifs.read(buffer.data(), fileSize);
        ifs.close();

        return buffer;
    }

}
