#include "utils/vulkan/helpers.hpp"


namespace utils::vulkan {

    std::vector<VkExtensionProperties> getSupportedExtensions() {
        uint32_t vkExtensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &vkExtensionCount, nullptr);

        std::vector<VkExtensionProperties> vkExtensions(vkExtensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &vkExtensionCount, vkExtensions.data());

        return vkExtensions;
    }


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

}
