#include "utils/vulkan/vertex_buffer.hpp"

#include "vulkan/vulkan.h"


namespace utils::vulkan {

    VertexBuffer::VertexBuffer(
        std::shared_ptr<DeviceHandle> const& vkDeviceHandle,
        uint64_t const size,
        VkSharingMode const sharingMode
    ) :
        HandleWrapper<BufferHandle>(std::make_shared<BufferHandle>(vkDeviceHandle)),
        vkDeviceHandle(vkDeviceHandle)
    {
        VkBufferCreateInfo createInfo {};
        createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        createInfo.size = size;
        createInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        createInfo.sharingMode = sharingMode;

        if (vkCreateBuffer(this->vkDeviceHandle->vk, &createInfo, nullptr, &this->vkHandle->vk) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create vertex buffer.");
        }
    }


    VkMemoryRequirements VertexBuffer::getMemoryRequirements() const {
        VkMemoryRequirements requirements;
        vkGetBufferMemoryRequirements(this->vkDeviceHandle->vk, this->vkHandle->vk, &requirements);
        return requirements;
    }
}
