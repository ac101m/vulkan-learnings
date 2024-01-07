#pragma once

#include "vulkan/vulkan.h"


namespace utils::vulkan {

    class PhysicalDevice {
    private:
        VkPhysicalDevice const vkDevice;
        uint32_t score;

        uint32_t computeScore() const;

    public:
        PhysicalDevice(VkPhysicalDevice const& vkDevice);

        VkPhysicalDeviceProperties getProperties() const;
        VkPhysicalDeviceFeatures getFeatures() const;

        bool operator<(PhysicalDevice const& other) const {
            return score < other.getScore();
        }

        uint32_t getScore() const {
            return score;
        }
    };

}
