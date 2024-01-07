#include "utils/vulkan/physical_device.hpp"


namespace utils::vulkan {

    QueueFamily::QueueFamily(uint32_t const index, VkQueueFamilyProperties const properties) :
        index(index), properties(properties) {}


    PhysicalDevice::PhysicalDevice(VkPhysicalDevice const& vkPhysicalDevice) : vkPhysicalDevice(vkPhysicalDevice) {}


    VkPhysicalDeviceProperties PhysicalDevice::getProperties() const {
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(vkPhysicalDevice, &deviceProperties);
        return deviceProperties;
    }


    VkPhysicalDeviceFeatures PhysicalDevice::getFeatures() const {
        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceFeatures(vkPhysicalDevice, &deviceFeatures);
        return deviceFeatures;
    }


    std::vector<VkQueueFamilyProperties> PhysicalDevice::getQueueFamilyProperties() const {
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice, &queueFamilyCount, queueFamilies.data());

        return queueFamilies;
    }


     std::optional<QueueFamily> PhysicalDevice::selectQueueFamily(uint32_t const requiredQueueFlags) const {
        auto const queueFamilyProperties = getQueueFamilyProperties();

        for (unsigned i = 0; i < queueFamilyProperties.size(); i++) {
            auto const& properties = queueFamilyProperties[i];

            // Check if queue family supports
            if ((properties.queueFlags & requiredQueueFlags) == requiredQueueFlags) {
                return QueueFamily(i, properties);
            }
        }

        return std::optional<QueueFamily>();
    }


    uint32_t PhysicalDevice::getScore(uint32_t const requiredQueueFlags) const {
        uint32_t score = 0;

        auto const features = getFeatures();

        // Can't do anything without geometry!'
        if (!features.geometryShader) {
            return 0;
        }

        auto const queueFamily = selectQueueFamily(requiredQueueFlags);

        // Missing queue families
        if (!queueFamily.has_value()) {
            return 0;
        }

        auto const properties = getProperties();

        // Strongly prefer discrete GPUs
        if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            score += 10000;
        }

        return score;
    }

}
