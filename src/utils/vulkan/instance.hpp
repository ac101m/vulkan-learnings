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

    public:
        Instance(std::vector<std::string> const& validationLayers);
        ~Instance();

        VkInstance& get() {
            return vkInstance;
        }
    };

}
