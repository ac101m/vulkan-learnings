#pragma once

#include "utils/vulkan/debug.hpp"

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
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

        uint32_t const logMessageTypes =
             VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
             VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
             VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

    public:
        Instance(std::vector<std::string> const& validationLayers);
        ~Instance();

        VkInstance& get() {
            return vkInstance;
        }
    };

}
