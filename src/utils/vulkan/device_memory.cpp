#include "utils/vulkan/device_memory.hpp"


namespace utils::vulkan {

    utils::Logger DeviceMemory::log("DeviceMemory");


    DeviceMemory::DeviceMemory(
        std::shared_ptr<DeviceHandle> const& vkDeviceHandle,
        uint32_t const memoryTypeIndex,
        uint64_t const allocationSize
    ) :
        HandleWrapper<DeviceMemoryHandle>(std::make_shared<DeviceMemoryHandle>(vkDeviceHandle)),
        vkDeviceHandle(vkDeviceHandle),
        type(memoryTypeIndex),
        size(allocationSize)
    {
        VkMemoryAllocateInfo allocInfo {};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.memoryTypeIndex = memoryTypeIndex;
        allocInfo.allocationSize = allocationSize;

        if (vkAllocateMemory(this->vkDeviceHandle->vk, &allocInfo, nullptr, &this->vkHandle->vk) != VK_SUCCESS) {
            throw std::runtime_error("Failed to allocate device memory.");
        }
    }

}
