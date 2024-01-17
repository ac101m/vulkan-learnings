#include "utils/vulkan/command_pool.hpp"

#include "vulkan/vulkan.h"


namespace utils::vulkan {

    utils::Logger CommandPool::log("CommandPool");


    CommandPool::CommandPool(
        std::shared_ptr<DeviceHandle> const& vkDeviceHandle,
        CommandPoolConfig const& config
    ) :
        HandleWrapper<CommandPoolHandle>(std::make_shared<CommandPoolHandle>(vkDeviceHandle)),
        vkDeviceHandle(vkDeviceHandle)
    {
        INFO(log) << "Creating command pool." << std::endl;

        VkCommandPoolCreateInfo createInfo {};
        createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        createInfo.flags = config.flagBits;
        createInfo.queueFamilyIndex = config.queueFamilyIndex;

        if (vkCreateCommandPool(this->vkDeviceHandle->vk, &createInfo, nullptr, &this->vkHandle->vk) != VK_SUCCESS) {
            throw std::runtime_error("failed to create command pool.");
        }
    }


    std::shared_ptr<CommandBuffer> CommandPool::allocateCommandBuffer(VkCommandBufferLevel const flags) const {
        return std::make_shared<CommandBuffer>(this->vkDeviceHandle, this->vkHandle, flags);
    }

}
