#pragma once

#include "utils/misc/logging.hpp"

#include <stb/stb_image.h>

#include <stdint.h>
#include <filesystem>


namespace utils {

    class Image {
    private:
        static utils::Logger log;

        int m_width;
        int m_height;
        int m_channels;

        stbi_uc * m_data;
        uint32_t const m_format;

    private:
        static uint32_t getChannelCount(uint32_t const format);

    public:
        Image(std::filesystem::path const& path, uint32_t const format);
        ~Image();

        uint64_t dataSize() {
            return m_width * m_height * getChannelCount(m_format);
        }

        uint32_t width() {
            return m_width;
        }

        uint32_t height() {
            return m_height;
        }

        uint32_t channels() {
            return m_channels;
        }

        stbi_uc * data() {
            return m_data;
        }
    };

}
