#include "utils/vulkan/physical_device.hpp"

#include <set>
#include <algorithm>


namespace utils::vulkan {

    utils::Logger PhysicalDevice::log("PhysicalDevice");


    QueueFamily::QueueFamily(uint32_t const index, VkQueueFamilyProperties const properties) :
        index(index), properties(properties) {}


    PhysicalDevice::PhysicalDevice(
        std::shared_ptr<InstanceHandle> const& vkInstanceHandle, VkPhysicalDevice const& vkPhysicalDevice
    ) : vkInstanceHandle(vkInstanceHandle), vkPhysicalDevice(vkPhysicalDevice) {}


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


    std::optional<QueueFamily> PhysicalDevice::selectQueueFamily(QueueConstraints const& queueConstraints) const {
        auto const queueFamilyProperties = getQueueFamilyProperties();

        for (unsigned i = 0; i < queueFamilyProperties.size(); i++) {
            auto const& properties = queueFamilyProperties[i];

            // Check if queue family supports required queue operations
            if ((properties.queueFlags & queueConstraints.requiredFlags) != queueConstraints.requiredFlags) {
                continue;
            }

            // Check present requirements if provided
            if (queueConstraints.presentSurface != nullptr) {
                VkBool32 supported = false;

                vkGetPhysicalDeviceSurfaceSupportKHR(
                    this->vkPhysicalDevice, i,
                    queueConstraints.presentSurface->getHandle()->vk,
                    &supported);

                if (!supported) {
                    continue;
                }
            }

            return QueueFamily(i, properties);
        }

        return std::optional<QueueFamily>();
    }


    bool PhysicalDevice::checkExtensionSupport(std::vector<std::string> const& requiredExtensions) const {
        if (requiredExtensions.size() == 0) {
            return true;
        }

        uint32_t extensionCount = 0;
        vkEnumerateDeviceExtensionProperties(this->vkPhysicalDevice, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> supportedExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(this->vkPhysicalDevice, nullptr, &extensionCount, supportedExtensions.data());

        for (auto const& requiredExtension : requiredExtensions) {
            bool found = false;

            for (auto const& supportedExtension : supportedExtensions) {
                if (requiredExtension == supportedExtension.extensionName) {
                    found = true;
                    break;
                }
            }

            if (!found) {
                return false;
            }
        }

        return true;
    }


    uint32_t PhysicalDevice::getScore(QueuePlan const& queuePlan, std::vector<std::string> const& requiredExtensions) const {
        uint32_t score = 0;

        auto const features = getFeatures();

        // Can't draw anything without geometry!'
        if (!features.geometryShader) {
            return 0;
        }

        // Check for support of required extensions
        if (!checkExtensionSupport(requiredExtensions)) {
            return 0;
        }

        // Check that all the required queue constraints are met
        for (auto const& entry : queuePlan.queues) {
            auto const& queueConstraints = entry.second;

            // No queue family with apropriate features found!
            if (!selectQueueFamily(queueConstraints).has_value()) {
                return 0;
            }
        }

        auto const properties = getProperties();

        // Strongly prefer discrete GPUs
        if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
            score += 10000;
        }

        return score;
    }


    std::shared_ptr<Device> PhysicalDevice::createLogicalDevice(
        QueuePlan const& queuePlan,
        std::vector<std::string> const& deviceExtensions
    ) const {
        std::map<std::string, uint32_t> queueFamilyIndexMap;

        for (auto const& entry : queuePlan.queues) {
            auto const& queueName = entry.first;
            auto const& queueConstraints = entry.second;

            auto const queueFamily = selectQueueFamily(queueConstraints);

            if (!queueFamily.has_value()) {
                throw std::runtime_error("Device does not support required queues.");
            }

            queueFamilyIndexMap[queueName] = queueFamily.value().index;
        }

        return std::make_shared<Device>(this->vkInstanceHandle, this->vkPhysicalDevice, queueFamilyIndexMap, deviceExtensions);
    }

}
