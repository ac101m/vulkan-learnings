#pragma once

#include "vulkan/vulkan.h"

#include <vector>
#include <optional>


namespace utils::vulkan {

    class QueueFamilyIndices {
    public:
        std::optional<uint32_t> graphicsFamily;

        bool allPresent() const {
            return graphicsFamily.has_value();
        }
    };


    class PhysicalDevice {
    private:
        VkPhysicalDevice const vkDevice;
        uint32_t score;

        uint32_t computeScore() const;

    public:
        PhysicalDevice(VkPhysicalDevice const& vkDevice);

        VkPhysicalDeviceProperties getProperties() const;
        VkPhysicalDeviceFeatures getFeatures() const;

        /**
         * @brief Get list of queue family properties for this device.
         * @return std::vector of VkQueueFamilyProperties instances.
         */
        std::vector<VkQueueFamilyProperties> getQueueFamilyProperties() const;

        /**
         * @brief Get a list of queue family indices which support required features.
         * @return QueueFamilyIndices object containing indices of needed queues.
         */
        QueueFamilyIndices findQueueFamilies() const;

        /**
         * For comparison of devices based on suitability score.
         */
        bool operator<(PhysicalDevice const& other) const {
            return score < other.getScore();
        }

        /**
         * @brief Get a device suitability score based on device characteristics.
         * @return Device suitability score as uint32_t, higher is better.
         */
        uint32_t getScore() const {
            return score;
        }
    };

}
