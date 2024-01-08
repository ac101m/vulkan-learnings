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
        QueueFamily const queueFamily;

        static utils::Logger log;

    public:
        /**
         * @brief Construct and initialize a new logical device instance.
         * @param vkInstanceHandle std::shared_ptr to utils::vulkan::Instance.
         * @param physicalDevice utils::vulkan::PhysicalDevice instance designating the desired GPU.
         * @param queueFamily Queue family to use for logical device.
         */
        Device(std::shared_ptr<Instance> vkInstanceHandle, PhysicalDevice& physicalDevice, QueueFamily const& queueFamily);
        ~Device();

        VkDevice& get() {
            return vkDevice;
        }
    };

}
