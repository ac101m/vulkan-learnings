#include "utils/vulkan/buffer.hpp"

#include "vulkan/vulkan.h"


namespace utils::vulkan {

    Buffer::Buffer(
        std::shared_ptr<DeviceHandle> const& vkDeviceHandle,
        uint64_t const size,
        VkMemoryPropertyFlags const usageFlags,
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


    VkMemoryRequirements Buffer::getMemoryRequirements() const {
        VkMemoryRequirements requirements;
        vkGetBufferMemoryRequirements(this->vkDeviceHandle->vk, this->vkHandle->vk, &requirements);
        return requirements;
    }


    void Buffer::bindMemory(std::shared_ptr<DeviceMemory> const& deviceMemory, uint64_t const offset) {
        auto const requirements = this->getMemoryRequirements();

        if (offset % requirements.alignment != 0) {
            throw std::runtime_error("Unable to bind vertex buffer to device memory, offset is not aligned.");
        }

        if (offset + requirements.size > deviceMemory->size) {
            throw std::runtime_error("Unable to bind vertex buffer to device memory, memory is too small.");
        }

        this->vkDeviceMemory = deviceMemory->getHandle();
        this->memoryOffset = offset;
        this->memorySize = requirements.size;

        if (vkBindBufferMemory(this->vkDeviceHandle->vk, this->vkHandle->vk, this->vkDeviceMemory->vk, offset) != VK_SUCCESS) {
            throw std::runtime_error("Failed to bind vertex buffer memory.");
        }
    }


    void Buffer::mapMemory() {
        if (vkMapMemory(this->vkDeviceHandle->vk, this->vkDeviceMemory->vk, this->memoryOffset, this->memorySize, 0, &this->data) != VK_SUCCESS) {
            throw std::runtime_error("Failed to map vertex buffer memory.");
        }
    }


    void Buffer::unmapMemory() {
        vkUnmapMemory(this->vkDeviceHandle->vk, this->vkDeviceMemory->vk);
        this->data = nullptr;
    }
}
