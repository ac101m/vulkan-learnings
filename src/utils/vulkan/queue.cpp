#include "utils/vulkan/queue.hpp"


namespace utils::vulkan {

    Queue::Queue(
        std::shared_ptr<DeviceHandle> const& vkDeviceHandle,
        uint32_t const queueFamilyIndex,
        uint32_t const queueIndex
    ) : vkDeviceHandle(vkDeviceHandle) {
        vkGetDeviceQueue(vkDeviceHandle->vk, queueFamilyIndex, queueIndex, &vkQueue);
    }

}
