#pragma once

#include "vulkan/vulkan.h"

#include <memory>
#include <iostream>


namespace utils::vulkan {

    VkResult CreateDebugUtilsMessengerEXT(
        VkInstance const& instance,
        VkDebugUtilsMessengerCreateInfoEXT const * pCreateInfo,
        VkAllocationCallbacks const * pAllocator,
        VkDebugUtilsMessengerEXT * pDebugMessenger
    );


    void DestroyDebugUtilsMessengerEXT(
        VkInstance const& instance,
        VkDebugUtilsMessengerEXT debugMessenger,
        VkAllocationCallbacks const * pAllocator
    );


    VKAPI_ATTR VkBool32 VKAPI_CALL debugMessageCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        VkDebugUtilsMessengerCallbackDataEXT const * pCallbackData,
        void * pUserData
    );

}
