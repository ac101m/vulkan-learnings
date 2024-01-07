#pragma once

#include "utils/vulkan/physical_device.hpp"

#include "vulkan/vulkan.h"

#include <vector>
#include <optional>


namespace utils::vulkan {

    class Device {
    private:
        VkDevice vkDevice;

    public:
        Device(PhysicalDevice& physicalDevice, QueueFamily const& queueFamily);
        ~Device();

        VkDevice& get() {
            return vkDevice;
        }
    };

}
