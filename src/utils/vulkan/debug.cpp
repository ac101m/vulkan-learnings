#include "utils/vulkan/debug.hpp"
#include "utils/misc/logging.hpp"

#include <iostream>
#include <memory>


namespace utils::vulkan {

    VkResult CreateDebugUtilsMessengerEXT(
        VkInstance const& instance,
        VkDebugUtilsMessengerCreateInfoEXT const * pCreateInfo,
        VkAllocationCallbacks const * pAllocator,
        VkDebugUtilsMessengerEXT * pDebugMessenger
    ) {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

        if (func != nullptr) {
            return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
        } else {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }


    void DestroyDebugUtilsMessengerEXT(
        VkInstance const& instance,
        VkDebugUtilsMessengerEXT debugMessenger,
        VkAllocationCallbacks const * pAllocator
    ) {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");

        if (func != nullptr) {
            func(instance, debugMessenger, pAllocator);
        }
    }


    utils::Logger logger("Vulkan");


    VKAPI_ATTR VkBool32 VKAPI_CALL debugMessageCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        VkDebugUtilsMessengerCallbackDataEXT const * pCallbackData,
        void * pUserData
    ) {
        if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
            ERROR(logger) << pCallbackData->pMessage << std::endl;
        } else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
            WARN(logger) << pCallbackData->pMessage << std::endl;
        } else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) {
            INFO(logger) << pCallbackData->pMessage << std::endl;
        } else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
            TRACE(logger) << pCallbackData->pMessage << std::endl;
        }

        return VK_FALSE;
    }

}
