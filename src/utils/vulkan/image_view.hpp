#pragma once

#include "utils/misc/logging.hpp"
#include "utils/vulkan/handles.hpp"

#include <memory>


namespace utils::vulkan {

    struct ImageViewConfig {
        VkImageViewType viewType;
        VkFormat imageFormat;
    };


    class ImageView {
    private:
        static utils::Logger log;

        std::shared_ptr<ImageViewHandle> const vkImageViewHandle;
        std::shared_ptr<DeviceHandle> const vkDeviceHandle;
        std::shared_ptr<ImageHandle> const vkImageHandle;

    public:
        ImageViewConfig const config;

    public:
        ImageView(
            std::shared_ptr<DeviceHandle> const& vkDeviceHandle,
            std::shared_ptr<ImageHandle> const& vkImageHandle,
            ImageViewConfig const& config);
    };

}
