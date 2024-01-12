#pragma once

#include "utils/vulkan/helpers.hpp"
#include "utils/vulkan/device.hpp"

#include "vulkan/vulkan.h"

#include <vector>
#include <optional>
#include <memory>


namespace utils::vulkan {

    struct QueueFamily {
        uint32_t const index;
        VkQueueFamilyProperties const properties;

        QueueFamily(uint32_t const index, VkQueueFamilyProperties const properties);
    };


    class PhysicalDevice {
    private:
        static utils::Logger log;

        std::shared_ptr<InstanceHandle> vkInstanceHandle;

        VkPhysicalDevice vkPhysicalDevice;

    public:
        PhysicalDevice(std::shared_ptr<InstanceHandle> const& vkInstanceHandle, VkPhysicalDevice const& vkPhysicalDevice);

        VkPhysicalDeviceProperties getProperties() const;
        VkPhysicalDeviceFeatures getFeatures() const;

        /**
         * @brief Get list of queue family properties for this device.
         * @return std::vector of VkQueueFamilyProperties instances.
         */
        std::vector<VkQueueFamilyProperties> getQueueFamilyProperties() const;

        /**
         * @brief Select a queue family based on required queue properties.
         * @param queueConstraints QueueConstraints object specifying required queue properties.
         * @return std::optional of QueueFamily object which contains the queue
         * family index and it's properties.
         */
        std::optional<QueueFamily> selectQueueFamily(QueueConstraints const& queueConstraints) const;

        /**
         * @brief Check if this device supports provided extensions.
         * @param requiredExtensions vector of strings containing the names of required extensions.
         * @return boolean true if all extensions are supported, false otherwise.
         */
        bool checkExtensionSupport(std::vector<std::string> const& requiredExtensions) const;

        /**
         * @brief Get a device suitability score based on device characteristics.
         * @param queuePlan Queue plan object containing set of required queues.
         * @return Device suitability score as uint32_t, higher is better. 0 means device is unsuitable.
         */
        uint32_t getScore(QueuePlan const& queuePlan, std::vector<std::string> const& requiredExtensions) const;

        /**
         * @brief Create a logical device from this physical device.
         * @param queueFamily QueueFamily object specifying family to use.
         * @param deviceExtensions Vector of strings containing the names of required extensions, defaults to empty vector.
         */
        std::shared_ptr<utils::vulkan::Device> createLogicalDevice(
            QueuePlan const& queuePlan,
            std::vector<std::string> const& deviceExtensions) const;
    };

}
