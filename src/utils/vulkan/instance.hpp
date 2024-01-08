#pragma once

#include "utils/vulkan/debug.hpp"
#include "utils/vulkan/physical_device.hpp"
#include "utils/misc/logging.hpp"

#include "vulkan/vulkan.h"

#include <vector>
#include <string>
#include <memory>


namespace utils::vulkan {

    class Instance {
    private:
        VkInstance vkInstance;
        VkDebugUtilsMessengerEXT vkDebugMessenger;

        bool const debugEnabled;

        std::vector<std::string> getRequiredExtensions(std::vector<std::string> const& validationLayers) const;
        void checkValidationLayerSupport(std::vector<std::string> const& checkValidationLayerSupport) const;

        uint32_t const logMessageSeverities =
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

        uint32_t const logMessageTypes =
            VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

        static utils::Logger log;

    public:
        /**
         * @brief Construct and initialize a new vulkan instance.
         * @param validationLayers std::vector of strings specifying desired validation layers.
         */
        Instance(std::vector<std::string> const& validationLayers);
        ~Instance();

        /**
         * @brief Retrieve list of physical devices.
         * @return std::vector of PhysicalDevice objects.
         */
        std::vector<PhysicalDevice> getPhysicalDevices() const;

        /**
         * @brief Get the underlying vulkan C object.
         * @return Reference to underlying VkObject
         */
        VkInstance& get() {
            return vkInstance;
        }
    };

}
