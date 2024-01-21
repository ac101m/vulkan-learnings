#pragma once

#include "utils/vulkan/handles.hpp"
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


    class DebugMessenger : public HandleWrapper<DebugMessengerHandle> {
    private:
        static utils::Logger log;

        std::shared_ptr<InstanceHandle> const instanceHandle;

    public:
        DebugMessenger(
            std::shared_ptr<InstanceHandle> const& instanceHandle,
            uint32_t const logMessageSeverities,
            uint32_t const logMessageTypes);
    };

}
