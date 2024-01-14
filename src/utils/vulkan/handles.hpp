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
    class SurfaceHandle {
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
    class SwapChainHandle {
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


    /**
     * @brief Class for managing lifetime of VkImage objects.
     */
    class ImageHandle {
    private:
        std::shared_ptr<DeviceHandle> const& deviceHandle;
        std::shared_ptr<SwapChainHandle> const& owningSwapChain;

    public:
        VkImage_T * vk;

        ImageHandle(
            std::shared_ptr<DeviceHandle> const& deviceHandle,
            std::shared_ptr<SwapChainHandle> const& owningSwapChain = nullptr
        ) : deviceHandle(deviceHandle), owningSwapChain(owningSwapChain) {}

        ~ImageHandle() {
            if (owningSwapChain == nullptr) {
                vkDestroyImage(this->deviceHandle->vk, this->vk, nullptr);
            }
        }
    };


    /**
     * @brief Class for managing lifetime of VkImageView objects.
     */
    class ImageViewHandle {
    private:
        std::shared_ptr<DeviceHandle> const deviceHandle;

    public:
        VkImageView_T * vk;

        ImageViewHandle(std::shared_ptr<DeviceHandle> const deviceHandle) :
            deviceHandle(deviceHandle) {}

        ~ImageViewHandle() {
            vkDestroyImageView(this->deviceHandle->vk, this->vk, nullptr);
        }
    };


    /**
     * @brief Class for managing lifetime of VkShaderModule objects.
     */
    class ShaderModuleHandle {
    private:
        std::shared_ptr<DeviceHandle> const vkDeviceHandle;

    public:
        VkShaderModule_T * vk;

        ShaderModuleHandle(std::shared_ptr<DeviceHandle> const& vkDeviceHandle) :
            vkDeviceHandle(vkDeviceHandle) {}

        ~ShaderModuleHandle() {
            vkDestroyShaderModule(this->vkDeviceHandle->vk, this->vk, nullptr);
        }
    };
}
