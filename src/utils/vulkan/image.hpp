#pragma once

#include "utils/misc/logging.hpp"
#include "utils/vulkan/handles.hpp"
#include "utils/vulkan/image_view.hpp"
#include "utils/vulkan/device_memory.hpp"

#include "vulkan/vulkan.h"

#include <memory>


namespace utils::vulkan {

    /**
     * @brief Contains mutable image settings.
     */
    struct MutableImageSettings {
        VkImageLayout layout;
        uint32_t mipLevelCount;
        uint32_t layerCount;
    };


    /**
     * @brief Helper object for configuring image objects.
     */
    struct ImageConfig {

        // Mandatory parameters
        VkImageType const type;
        uint32_t const width;
        uint32_t const height;
        uint32_t const depth;

        // Parameters with defaults
        uint32_t mipLevelCount = 1;
        uint32_t layerCount = 1;
        VkFormat format = VK_FORMAT_R8G8B8A8_SRGB;
        VkImageTiling tiling = VK_IMAGE_TILING_OPTIMAL;
        VkImageLayout initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        VkImageUsageFlags usage = 0;
        VkSharingMode sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT;

        /**
         * @brief Construct a new image config.
         * @param type Type of the image (e.g. VK_IMAGE_TYPE_2D)
         * @param width Width of the image in pixels.
         * @param height Height of the image in pixels.
         * @param depth Depth of the image in pixels.
         */
        ImageConfig(
            VkImageType const type,
            uint32_t const width,
            uint32_t const height,
            uint32_t const depth = 1
        ) : type(type), width(width), height(height), depth(depth) {}

        /**
         * @brief Set the image format for the configuration.
         * @param newFormat Format to set in the configuration.
         */
        ImageConfig& setFormat(VkFormat const newFormat) {
            format = newFormat;
            return *this;
        }

        /**
         * @brief Set a usage flag for the configuration.
         * @param flag Flag to add to the usage flags.
         */
        ImageConfig& setUsageFlag(VkImageUsageFlags const flag) {
            usage |= flag;
            return *this;
        }

        /**
         * @brief Clear a usage flag from the configuration.
         * @param flag Flag to clear from the usage flags.
         */
        ImageConfig& clearUsageFlag(VkImageUsageFlags const flag) {
            usage &= ~flag;
            return *this;
        }

        /**
         * @brief Get mutable image settings settings.
         * @return Mutable image settings structure.
         */
        MutableImageSettings getMutableSettings() const {
            MutableImageSettings settings {};
            settings.layout = this->initialLayout;
            settings.mipLevelCount = this->mipLevelCount;
            settings.layerCount = this->layerCount;
            return settings;
        }
    };


    class Image : public HandleWrapper<ImageHandle> {
    private:
        static utils::Logger log;

        std::shared_ptr<DeviceHandle> const vkDeviceHandle;

        MutableImageSettings mutableSettings;

    public:
        Image(std::shared_ptr<ImageHandle> const& vkImageHandle, std::shared_ptr<DeviceHandle> const& vkDeviceHandle);
        Image(std::shared_ptr<DeviceHandle> const& vkDeviceHandle, ImageConfig const& config);

        /**
         * @brief Create an image view for the specified image.
         * @param config ImageViewConfig object containing image view configuration details.
         * @return Shared pointer to ImageView object.
         */
        std::shared_ptr<ImageView> createImageView(ImageViewConfig const& config);

        /**
         * @brief Get image memory requirements.
         * @return VkMemoryRequirements object containing memory requirements.
         */
        VkMemoryRequirements getMemoryRequirements();

        /**
         * @brief Bind memory to the image.
         * @param deviceMemory Device memory to bind to the image.
         * @param offset Offset within
         */
        void bindMemory(std::shared_ptr<DeviceMemory> const& deviceMemory, uint64_t const offset = 0);

        /**
         * @brief Get mutable image settings.
         * @return Mutable image settings structure.
         */
        MutableImageSettings getMutableSettings() const { return this->mutableSettings; }

        /**
         * @brief Update image metadata based on barrier details.
         * @param newSettings New mutable image settings.
         * @return Image memory barrier required to update mutable image settings.
         */
        VkImageMemoryBarrier updateSettings(MutableImageSettings const& newSettings);

    };

}
