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


    std::vector<VkQueueFamilyProperties> PhysicalDevice::getQueueFamilyProperties() const {
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(vkDevice, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(vkDevice, &queueFamilyCount, queueFamilies.data());

        return queueFamilies;
    }


    QueueFamilyIndices PhysicalDevice::findQueueFamilies() const {
        QueueFamilyIndices indices;

        auto const queueFamilyProperties = getQueueFamilyProperties();

        for (unsigned i = 0; i < queueFamilyProperties.size(); i++) {
            auto const& properties = queueFamilyProperties[i];

            if (properties.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.graphicsFamily = i;
            }

            if (indices.allPresent()) {
                break;
            }
        }

        return indices;
    }


    uint32_t PhysicalDevice::computeScore() const {
        uint32_t score = 0;

        auto const features = getFeatures();

        // Can't do anything without geometry!'
        if (!features.geometryShader) {
            return 0;
        }

        auto const queueFamilyIndices = findQueueFamilies();

        // Missing queue families
        if (!queueFamilyIndices.allPresent()) {
            return 0;
        }

        auto const properties = getProperties();

        // Strongly prefer discreet GPUs
        if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            score += 10000;
        }

        return score;
    }

}
