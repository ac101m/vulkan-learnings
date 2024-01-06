#include "utils/vulkan/instance.hpp"
#include "utils/vulkan/utils.hpp"
#include "utils/vulkan/debug.hpp"

#include "GLFW/glfw3.h"

#include <stdexcept>
#include <vector>
#include <string>
#include <sstream>
#include <string.h>
#include <iostream>
#include <memory>


namespace utils::vulkan {

    Instance::Instance(std::vector<std::string> const& validationLayers) : debugEnabled(validationLayers.size() > 0) {
        if (debugEnabled) {
            checkValidationLayerSupport(validationLayers);
        }

        auto const requiredExtensions = getRequiredExtensions(validationLayers);

        VkApplicationInfo appInfo {};

        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Vulkan Experiment";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo {};

        auto const extensionParams = StringParameters(requiredExtensions);
        auto const validationLayerParams = StringParameters(validationLayers);

        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;
        createInfo.enabledExtensionCount = extensionParams.size();
        createInfo.ppEnabledExtensionNames = extensionParams.data();
        createInfo.enabledLayerCount = validationLayerParams.size();
        createInfo.ppEnabledLayerNames = validationLayerParams.data();

        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo {};

        if (debugEnabled) {
            debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
            debugCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
            debugCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
            debugCreateInfo.pfnUserCallback = debugMessageCallback;
            createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *) &debugCreateInfo;
        }

        if (vkCreateInstance(&createInfo, nullptr, &this->vkInstance) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create vulkan instance!");
        }

        if (debugEnabled) {
            VkDebugUtilsMessengerCreateInfoEXT createInfo {};

            createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
            createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
            createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
            createInfo.pfnUserCallback = debugMessageCallback;
            createInfo.pUserData = nullptr;

            if (CreateDebugUtilsMessengerEXT(this->vkInstance, &createInfo, nullptr, &this->vkDebugMessenger) != VK_SUCCESS) {
                throw std::runtime_error("Failed to set up debug messenger!");
            }
        }
    }


    Instance::~Instance() {
        if (debugEnabled) {
            DestroyDebugUtilsMessengerEXT(this->vkInstance, this->vkDebugMessenger, nullptr);
        }

        vkDestroyInstance(this->vkInstance, nullptr);
    }


    std::vector<std::string> Instance::getRequiredExtensions(std::vector<std::string> const& validationLayers) const {
        uint32_t glfwExtenstionCount = 0;
        char const ** glfwExtensions;

        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtenstionCount);

        std::vector<std::string> requiredExtensions(glfwExtenstionCount);

        for (unsigned i = 0; i < glfwExtenstionCount; i++) {
            requiredExtensions[i] = std::string(glfwExtensions[i]);
        }

        if (validationLayers.size() > 0) {
            requiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        return requiredExtensions;
    }


    void Instance::checkValidationLayerSupport(std::vector<std::string> const& validationLayers) const {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (std::string const& layerName : validationLayers) {
            bool layerFound = false;

            for (const auto& layerProperties : availableLayers) {
                if (strcmp(layerName.c_str(), layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound) {
                std::stringstream ss;
                ss << "Validation layer " << layerName << " not supported.";
                throw std::runtime_error(ss.str());
            }
        }
    }

}
