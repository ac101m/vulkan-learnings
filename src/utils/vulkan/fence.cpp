#include "utils/vulkan/fence.hpp"


namespace utils::vulkan {

    Fence::Fence(
        std::shared_ptr<DeviceHandle> const& vkDeviceHandle,
        VkFenceCreateFlagBits const flags
    ) :
        HandleWrapper<FenceHandle>(std::make_shared<FenceHandle>(vkDeviceHandle)),
        vkDeviceHandle(vkDeviceHandle)
    {
        VkFenceCreateInfo createInfo {};
        createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        createInfo.flags = flags;
        vkCreateFence(this->vkDeviceHandle->vk, &createInfo, nullptr, &this->vkHandle->vk);
    }


    void Fence::wait() {
        vkWaitForFences(this->vkDeviceHandle->vk, 1, &this->vkHandle->vk, VK_TRUE, UINT64_MAX);
    }


    void Fence::reset() {
        vkResetFences(this->vkDeviceHandle->vk, 1, &this->vkHandle->vk);
    }
}
