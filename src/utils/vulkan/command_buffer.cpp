#include "utils/vulkan/command_buffer.hpp"


namespace utils::vulkan {

    utils::Logger CommandBuffer::log("CommandBuffer");


    CommandBuffer::CommandBuffer(
        std::shared_ptr<DeviceHandle> const& vkDeviceHandle,
        std::shared_ptr<CommandPoolHandle> const& vkCommandPoolHandle,
        CommandBufferConfig const& config
    ) : vkDeviceHandle(vkDeviceHandle), vkCommandPoolHandle(vkCommandPoolHandle) {
        INFO(log) << "Creating command buffer." << std::endl;

        VkCommandBufferAllocateInfo createInfo {};
        createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        createInfo.commandPool = vkCommandPoolHandle->vk;
        createInfo.level = config.bufferLevel;
        createInfo.commandBufferCount = 1;

        if (vkAllocateCommandBuffers(this->vkDeviceHandle->vk, &createInfo, &this->vk) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate command buffers!");
        }
    }

}
