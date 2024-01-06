#include "utils/vulkan/debug.hpp"

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


    VKAPI_ATTR VkBool32 VKAPI_CALL debugMessageCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        VkDebugUtilsMessengerCallbackDataEXT const * pCallbackData,
        void * pUserData
    ) {
        std::cerr << pCallbackData->pMessage << "\n";
        return VK_FALSE;
    }

}
