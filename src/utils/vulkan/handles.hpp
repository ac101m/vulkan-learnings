#pragma once

#include "vulkan/vulkan.h"

#include <memory>
#include <string>


namespace utils::vulkan {

    /**
     * @brief Helper for vulkan utility classes which wrap a handle.
     * Used to reduce boilerplate a little.
     */
    template<typename T>
    class HandleWrapper {
    protected:
        std::shared_ptr<T> vkHandle;

    public:
        HandleWrapper(std::shared_ptr<T> const& vkHandle) :
            vkHandle(vkHandle) {}

        /**
         * @brief Retrieve handle of underlying vulkan object.
         * @return Shared pointer to handle object.
         */
        std::shared_ptr<T> getHandle() const {
            return vkHandle;
        }
    };


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

        /**
         * This constructor optionally take a pointer to the owning swap chain. If these are
         * swap chain images (which we didn't create ourselves), we should set this parameter to
         * keep the swap chain from being prematurely destroyed, and to signal that the image
         * should not be destroyed (the swap chain will clean up it's own images).
         */
        ImageHandle(
            std::shared_ptr<DeviceHandle> const& deviceHandle,
            std::shared_ptr<SwapChainHandle> const& owningSwapChain = nullptr
        ) : deviceHandle(deviceHandle), owningSwapChain(owningSwapChain) {}

        /**
         * As per comment above, only destroy if this is not a swap chain image!
         */
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


    /**
     * @brief Class for managing lifetime of VkPipelineLayout objects.
     */
    class PipelineLayoutHandle {
    private:
        std::shared_ptr<DeviceHandle> const vkDeviceHandle;

    public:
        VkPipelineLayout_T * vk;

        PipelineLayoutHandle(std::shared_ptr<DeviceHandle> const& vkDeviceHandle) :
            vkDeviceHandle(vkDeviceHandle) {}

        ~PipelineLayoutHandle() {
            vkDestroyPipelineLayout(this->vkDeviceHandle->vk, this->vk, nullptr);
        }
    };


    /**
     * @brief Class for managing lifetime of VkRenderPass objects.
     */
    class RenderPassHandle {
    private:
        std::shared_ptr<DeviceHandle> const vkDeviceHandle;

    public:
        VkRenderPass_T * vk;

        RenderPassHandle(std::shared_ptr<DeviceHandle> const& vkDeviceHandle) :
            vkDeviceHandle(vkDeviceHandle) {}

        ~RenderPassHandle() {
            vkDestroyRenderPass(this->vkDeviceHandle->vk, this->vk, nullptr);
        }
    };


    /**
     * @brief Class for managing lifetime of VkPipeline objects.
     */
    class PipelineHandle {
    private:
        std::shared_ptr<DeviceHandle> const vkDeviceHandle;

    public:
        VkPipeline_T * vk;

        PipelineHandle(std::shared_ptr<DeviceHandle> const& vkDeviceHandle) :
            vkDeviceHandle(vkDeviceHandle) {}

        ~PipelineHandle() {
            vkDestroyPipeline(this->vkDeviceHandle->vk, this->vk, nullptr);
        }
    };


    /**
     * @brief Class for managing lifetime of VkFrameBuffer objects.
     */
    class FrameBufferHandle {
    private:
        std::shared_ptr<DeviceHandle> const vkDeviceHandle;

    public:
        VkFramebuffer_T * vk;

        FrameBufferHandle(std::shared_ptr<DeviceHandle> const& vkDeviceHandle) :
            vkDeviceHandle(vkDeviceHandle) {}

        ~FrameBufferHandle() {
            vkDestroyFramebuffer(this->vkDeviceHandle->vk, this->vk, nullptr);
        }
    };
}
