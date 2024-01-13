#include "utils/vulkan/queue.hpp"


namespace utils::vulkan {

    utils::Logger Queue::log("Queue");


    Queue::Queue(
        std::shared_ptr<DeviceHandle> const& vkDeviceHandle,
        uint32_t const queueFamilyIndex,
        uint32_t const queueIndex,
        std::string const& name
    ) : vkDeviceHandle(vkDeviceHandle), name(name), queueFamilyIndex(queueFamilyIndex) {
        INFO(log) << "Creating queue '" << name << "' family index " << queueFamilyIndex << std::endl;
        vkGetDeviceQueue(vkDeviceHandle->vk, queueFamilyIndex, queueIndex, &vkQueue);
    }

}
