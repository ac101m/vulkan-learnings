#pragma once

#include "utils/vulkan/utils.hpp"

#include "utils/misc/logging.hpp"

#include "vulkan/vulkan.h"

#include <vector>
#include <optional>
#include <memory>


namespace utils::vulkan {

    class Device {
    private:
        static utils::Logger log;

        std::shared_ptr<InstanceHandle> vkInstanceHandle;
        std::shared_ptr<DeviceHandle> vkDeviceHandle;

    public:
        /**
         * @brief Construct and initialize a new logical device instance.
         * @param vkInstanceHandle std::shared_ptr to utils::vulkan::Instance.
         * @param physicalDevice VkPhysicalDevice object instance designating the desired GPU.
         * @param queueFamilyIndex Index of the queue family to use for this logical device.
         * @param queueCount Number of queues to create.
         * @param validationLayerNames std::vector of std::string containing device validation layers (optional).
         */
        Device(
            std::shared_ptr<InstanceHandle> const& vkInstanceHandle,
            VkPhysicalDevice const& physicalDevice,
            uint32_t const queueFamilyIndex,
            uint32_t const queueCount,
            std::vector<std::string> const& validationLayerNames = std::vector<std::string>());
    };

}
