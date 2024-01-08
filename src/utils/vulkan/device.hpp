#pragma once

#include "utils/vulkan/physical_device.hpp"
#include "utils/vulkan/instance.hpp"
#include "utils/misc/logging.hpp"

#include "vulkan/vulkan.h"

#include <vector>
#include <optional>
#include <memory>


namespace utils::vulkan {

    class Device {
    private:
        VkDevice vkDevice;

        std::shared_ptr<Instance> vkInstanceHandle;

        static utils::Logger log;

    public:
        Device(std::shared_ptr<Instance> vkInstanceHandle, PhysicalDevice& physicalDevice, QueueFamily const& queueFamily);
        ~Device();

        VkDevice& get() {
            return vkDevice;
        }
    };

}
