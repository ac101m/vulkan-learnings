#pragma once

#include "utils/vulkan/physical_device.hpp"

#include "vulkan/vulkan.h"

#include <vector>
#include <optional>


namespace utils::vulkan {

    class Device {
    private:
        //VkDevice const vkDevice;

    public:
        Device(PhysicalDevice const& physicalDevice);
        ~Device();
    };

}
