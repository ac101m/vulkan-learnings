#pragma once

#include "utils/misc/logging.hpp"
#include "utils/vulkan/helpers.hpp"

#include <memory>


namespace utils::vulkan {

    class Queue {
    private:
        static utils::Logger log;

        std::shared_ptr<DeviceHandle> const vkDeviceHandle;

        VkQueue_T * vkQueue;

    public:
        Queue(std::shared_ptr<DeviceHandle> const& vkDeviceHandle, uint32_t const queueFamilyIndex, uint32_t const queueIndex);
    };

}
