#pragma once

#include "utils/vulkan/helpers.hpp"
#include "utils/vulkan/queue.hpp"
#include "utils/vulkan/swap_chain.hpp"

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

        std::map<std::string, std::shared_ptr<Queue>> queueMap;

    private:
        void populateQueueMap(std::map<uint32_t, std::vector<std::string>> const& queueFamilyMap);

    public:
        /**
         * @brief Construct and initialize a new logical device instance.
         * @param vkInstanceHandle std::shared_ptr to utils::vulkan::Instance.
         * @param physicalDevice VkPhysicalDevice object instance designating the desired GPU.
         * @param queueFamilyIndexMap Map of queue names to queue family indices.
         * @param deviceExtensions Vector of strings specifying required device extensions.
         * @param validationLayerNames Vector of strings containing device validation layers (optional).
         */
        Device(
            std::shared_ptr<InstanceHandle> const& vkInstanceHandle,
            VkPhysicalDevice const& physicalDevice,
            std::map<std::string, uint32_t> const& queueFamilyIndexMap,
            std::vector<std::string> const& deviceExtensions,
            std::vector<std::string> const& validationLayerNames = std::vector<std::string>());

        /**
         * @brief Get a device queue.
         * @param name Name of the queue to retrieve (as passed in constructor map).
         * @return std::shared_ptr to a Queue object.
         */
        std::shared_ptr<Queue> getQueue(std::string const& queueName);

        /**
         * @brief Create a new swap chain.
         */
        std::shared_ptr<SwapChain> createSwapChain(
            std::shared_ptr<Surface> const& surface,
            SwapChainConfig const& swapChainConfig) const;
    };

}
