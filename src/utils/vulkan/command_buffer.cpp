#include "utils/vulkan/command_buffer.hpp"


namespace utils::vulkan {

    utils::Logger CommandBuffer::log("CommandBuffer");


    CommandBuffer::CommandBuffer(
        std::shared_ptr<DeviceHandle> const& vkDeviceHandle,
        std::shared_ptr<CommandPoolHandle> const& vkCommandPoolHandle,
        VkCommandBufferUsageFlagBits const bufferLevel
    ) : vkDeviceHandle(vkDeviceHandle), vkCommandPoolHandle(vkCommandPoolHandle) {
        INFO(log) << "Creating command buffer." << std::endl;

        VkCommandBufferAllocateInfo createInfo {};
        createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        createInfo.commandPool = vkCommandPoolHandle->vk;
        createInfo.level = bufferLevel;
        createInfo.commandBufferCount = 1;

        if (vkAllocateCommandBuffers(this->vkDeviceHandle->vk, &createInfo, &this->vk) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate command buffer.");
        }
    }


    void CommandBuffer::beginCommandBuffer(VkCommandBufferUsageFlagBits const flags) {
        VkCommandBufferBeginInfo beginInfo {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = flags;
        beginInfo.pInheritanceInfo = nullptr; // Optional

        if (vkBeginCommandBuffer(this->vk, &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("failed to begin recording command buffer.");
        }
    }

}
