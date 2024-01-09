#include "utils/vulkan/debug.hpp"

#include <iostream>
#include <memory>


namespace utils::vulkan {

    utils::Logger log = utils::Logger("Vulkan");


    VKAPI_ATTR VkBool32 VKAPI_CALL debugMessageCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        VkDebugUtilsMessengerCallbackDataEXT const * pCallbackData,
        void * pUserData
    ) {
        if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
            ERROR(log) << pCallbackData->pMessage << std::endl;
        } else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
            WARN(log) << pCallbackData->pMessage << std::endl;
        } else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) {
            INFO(log) << pCallbackData->pMessage << std::endl;
        } else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
            TRACE(log) << pCallbackData->pMessage << std::endl;
        }

        return VK_FALSE;
    }


    VkResult DebugMessenger::CreateDebugUtilsMessengerEXT(
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


    void DebugMessenger::DestroyDebugUtilsMessengerEXT(
        VkInstance const& instance,
        VkDebugUtilsMessengerEXT debugMessenger,
        VkAllocationCallbacks const * pAllocator
    ) {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");

        if (func != nullptr) {
            func(instance, debugMessenger, pAllocator);
        }
    }


    utils::vulkan::DebugMessenger::DebugMessenger(
        std::shared_ptr<InstanceHandle> const& instanceHandle,
        uint32_t const logMessageSeverities,
        uint32_t const logMessageTypes
    ) :
        instanceHandle(instanceHandle)
    {
        VkDebugUtilsMessengerCreateInfoEXT createInfo {};

        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = logMessageSeverities;
        createInfo.messageType = logMessageTypes;
        createInfo.pfnUserCallback = debugMessageCallback;
        createInfo.pUserData = nullptr;

        if (CreateDebugUtilsMessengerEXT(this->instanceHandle->vk, &createInfo, nullptr, &this->vk) != VK_SUCCESS) {
            throw std::runtime_error("Failed to set up debug messenger!");
        }
    }


    DebugMessenger::~DebugMessenger() {
        DestroyDebugUtilsMessengerEXT(this->instanceHandle->vk, this->vk, nullptr);
    }

}
