#pragma once

#include "utils/misc/logging.hpp"
#include "utils/vulkan/handles.hpp"
#include "utils/vulkan/image_view.hpp"

#include "vulkan/vulkan.h"

#include <memory>


namespace utils::vulkan {

    class Image : public HandleWrapper<ImageHandle> {
    private:
        static utils::Logger log;

        std::shared_ptr<DeviceHandle> const vkDeviceHandle;

    public:
        Image(std::shared_ptr<ImageHandle> const& vkImageHandle, std::shared_ptr<DeviceHandle> const& vkDeviceHandle);

        /**
         * @brief Create an image view for the specified image.
         * @param config ImageViewConfig object containing image view configuration details.
         * @return Shared pointer to ImageView object.
         */
        std::shared_ptr<ImageView> createImageView(ImageViewConfig const& config);
    };

}
