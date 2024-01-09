#pragma once

#include "utils/vulkan/utils.hpp"
#include "utils/misc/logging.hpp"

#include "vulkan/vulkan.h"

#include <memory>
#include <iostream>


namespace utils::vulkan {

    VKAPI_ATTR VkBool32 VKAPI_CALL debugMessageCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        VkDebugUtilsMessengerCallbackDataEXT const * pCallbackData,
        void * pUserData
    );


    class DebugMessenger {
    private:
        static utils::Logger log;

        std::shared_ptr<InstanceHandle> const instanceHandle;

        VkDebugUtilsMessengerEXT_T * vk;

    private:
        static VkResult CreateDebugUtilsMessengerEXT(
            VkInstance const& instance,
            VkDebugUtilsMessengerCreateInfoEXT const * pCreateInfo,
            VkAllocationCallbacks const * pAllocator,
            VkDebugUtilsMessengerEXT * pDebugMessenger
        );


        static void DestroyDebugUtilsMessengerEXT(
            VkInstance const& instance,
            VkDebugUtilsMessengerEXT debugMessenger,
            VkAllocationCallbacks const * pAllocator
        );

    public:
        DebugMessenger(
            std::shared_ptr<InstanceHandle> const& instanceHandle,
            uint32_t const logMessageSeverities,
            uint32_t const logMessageTypes);

        ~DebugMessenger();
    };

}
