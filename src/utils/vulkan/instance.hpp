#pragma once

#include "utils/vulkan/handles.hpp"
#include "utils/vulkan/debug.hpp"
#include "utils/vulkan/physical_device.hpp"
#include "utils/vulkan/surface.hpp"
#include "utils/misc/logging.hpp"
#include "utils/glfw/window.hpp"

#include "vulkan/vulkan.h"

#include <vector>
#include <string>
#include <memory>


namespace utils::vulkan {

    class Instance {
    private:
        static utils::Logger log;

        std::shared_ptr<InstanceHandle> const instanceHandle;
        std::shared_ptr<DebugMessenger> debugMessenger = nullptr;

        bool const debugEnabled;
        std::vector<std::string> const validationLayers;

        uint32_t const logMessageSeverities =
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

        uint32_t const logMessageTypes =
            VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

    private:
        std::vector<std::string> getRequiredExtensions(std::vector<std::string> const& validationLayers) const;
        void checkValidationLayerSupport() const;

    public:
        /**
         * @brief Construct and initialize a new vulkan instance.
         * @param validationLayers std::vector of strings specifying desired validation layers.
         */
        Instance(std::vector<std::string> const& validationLayers);

        /**
         * @brief Create a window surface for rendering.
         * @param window Shared pointer to a glfw window object.
         * @return Shared pointer to Surface object.
         */
        std::shared_ptr<Surface> createWindowSurface(std::shared_ptr<utils::glfw::Window> const& window) const;

        /**
         * @brief Retrieve list of physical devices.
         * @return std::vector of shared pointers to PhysicalDevice objects.
         */
        std::vector<std::shared_ptr<PhysicalDevice>> getPhysicalDevices() const;

        /**
         * @brief Select a physical device based on device suitability and scoring.
         * @param queuePlan QueuePlan object containing set of required queues.
         * @param requiredExtensions Vector of strings specifying required device extensions (defaults to none).
         * @return shared pointer to physical device matching desired characteristics.
         */
        std::shared_ptr<PhysicalDevice> selectPhysicalDevice(
            QueuePlan const& queuePlan,
            std::vector<std::string> const& requiredExtensions = std::vector<std::string>()) const;
    };

}
