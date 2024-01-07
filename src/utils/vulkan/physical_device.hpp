#pragma once

#include "vulkan/vulkan.h"

#include <vector>
#include <optional>


namespace utils::vulkan {

    struct QueueFamily {
        uint32_t const index;
        VkQueueFamilyProperties const properties;

        QueueFamily(uint32_t const index, VkQueueFamilyProperties const properties);
    };


    class PhysicalDevice {
    private:
        VkPhysicalDevice vkPhysicalDevice;

    public:
        PhysicalDevice(VkPhysicalDevice const& vkPhysicalDevice);

        VkPhysicalDeviceProperties getProperties() const;
        VkPhysicalDeviceFeatures getFeatures() const;

        /**
         * @brief Get list of queue family properties for this device.
         * @return std::vector of VkQueueFamilyProperties instances.
         */
        std::vector<VkQueueFamilyProperties> getQueueFamilyProperties() const;

        /**
         * @brief Select a queue family based on required queue properties.
         * @return std::optional of QueueFamily object which contains the queue
         * family index and it's properties.
         */
        std::optional<QueueFamily> selectQueueFamily(uint32_t const requiredQueueFlags) const;

        /**
         * @brief Get a device suitability score based on device characteristics.
         * @return Device suitability score as uint32_t, higher is better. 0 means device is unsuitable.
         */
        uint32_t getScore(uint32_t const requiredQueueFlags) const;

        VkPhysicalDevice& get() {
            return vkPhysicalDevice;
        }
    };

}
