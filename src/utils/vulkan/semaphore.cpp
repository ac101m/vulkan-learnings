#include "utils/vulkan/semaphore.hpp"


namespace utils::vulkan {

    Semaphore::Semaphore(std::shared_ptr<DeviceHandle> const& vkDeviceHandle) :
        HandleWrapper<SemaphoreHandle>(std::make_shared<SemaphoreHandle>(vkDeviceHandle)),
        vkDeviceHandle(vkDeviceHandle)
    {
        VkSemaphoreCreateInfo createInfo {};
        createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        vkCreateSemaphore(this->vkDeviceHandle->vk, &createInfo, nullptr, &this->vkHandle->vk);
    }

}
