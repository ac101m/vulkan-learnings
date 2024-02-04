#include "utils/misc/image.hpp"


namespace utils {

    utils::Logger Image::log("Image");


    uint32_t Image::getChannelCount(uint32_t const format) {
        switch (format) {
            case STBI_grey: return 1;
            case STBI_grey_alpha: return 2;
            case STBI_rgb: return 3;
            case STBI_rgb_alpha: return 4;
            default:
                throw std::runtime_error("Unrecognised stb image format.");
        }
    }


    Image::Image(std::filesystem::path const& path, uint32_t const format) : m_format(format) {
        static std::string const errorPrefix = "Failed to load image data: ";

        INFO(log) << "Loading image " << path << std::endl;

        if (!std::filesystem::exists(path)) {
            throw std::runtime_error(errorPrefix + '\'' + path.string() + '\'' + ", no such file or directory.");
        }

        if (!std::filesystem::is_regular_file(path)) {
            throw std::runtime_error(errorPrefix + '\'' + path.string() + '\'' + ", not a regular file.");
        }

        m_data = stbi_load(path.c_str(), &m_width, &m_height, &m_channels, format);

        if (!m_data) {
            throw std::runtime_error(errorPrefix + '\'' + path.string() + '\'' + ", unknwon error.");
        }
    }


    Image::~Image() {
        stbi_image_free(m_data);
    }

}
