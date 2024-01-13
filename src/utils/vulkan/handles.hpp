#pragma once

#include "vulkan/vulkan.h"

#include <memory>
#include <string>


namespace utils::vulkan {

    /**
     * @brief Class for managing lifetime of VkInstance objects.
     */
    class InstanceHandle {
    public:
        VkInstance_T * vk;

        ~InstanceHandle() {
            vkDestroyInstance(this->vk, nullptr);
        }
    };


    /**
     * @brief Class for managing lifetime of VkDevice objects.
     */
    class DeviceHandle {
    public:
        VkDevice_T * vk;

        ~DeviceHandle() {
            vkDestroyDevice(this->vk, nullptr);
        }
    };


    /**
     * @brief Class for managing lifetime of VkSurface objects.
     */
    struct SurfaceHandle {
    private:
        std::shared_ptr<InstanceHandle> const instanceHandle;

    public:
        VkSurfaceKHR_T * vk;

        SurfaceHandle(std::shared_ptr<InstanceHandle> const& instanceHandle) :
            instanceHandle(instanceHandle) {}

        ~SurfaceHandle() {
            vkDestroySurfaceKHR(this->instanceHandle->vk, this->vk, nullptr);
        }
    };


    /**
     * @brief Class for managing lifetime of VkSwapChain objects.
     */
    struct SwapChainHandle {
    private:
        std::shared_ptr<DeviceHandle> const deviceHandle;

    public:
        VkSwapchainKHR_T * vk;

        SwapChainHandle(std::shared_ptr<DeviceHandle> const& deviceHandle) :
            deviceHandle(deviceHandle) {}

        ~SwapChainHandle() {
            vkDestroySwapchainKHR(this->deviceHandle->vk, this->vk, nullptr);
        }
    };
}