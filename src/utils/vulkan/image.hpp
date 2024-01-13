#pragma once

#include "utils/misc/logging.hpp"
#include "utils/vulkan/handles.hpp"

#include "vulkan/vulkan.h"

#include <memory>


namespace utils::vulkan {

    class Image {
    private:
        static utils::Logger log;

        VkImage const vkImage;

        std::shared_ptr<SwapChainHandle> const vkSwapChainHandle;

    public:
        Image(VkImage const& vkImage, std::shared_ptr<SwapChainHandle> const& vkSwapChainHandle);
    };

}
