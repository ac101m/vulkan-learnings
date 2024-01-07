#include "utils/vulkan/device.hpp"


namespace utils::vulkan {

    PhysicalDevice::PhysicalDevice(VkPhysicalDevice const& vkDevice) : vkDevice(vkDevice) {
        score = computeScore();
    }


    VkPhysicalDeviceProperties PhysicalDevice::getProperties() const {
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(vkDevice, &deviceProperties);
        return deviceProperties;
    }


    VkPhysicalDeviceFeatures PhysicalDevice::getFeatures() const {
        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceFeatures(vkDevice, &deviceFeatures);
        return deviceFeatures;
    }


    uint32_t PhysicalDevice::computeScore() const {
        uint32_t score = 0;

        auto const properties = getProperties();
        auto const features = getFeatures();

        // Can't do anything without geometry!'
        if (!features.geometryShader) {
            return 0;
        }

        // Strongly prefer discreet GPUs
        if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            score += 10000;
        }

        return score;
    }

}
