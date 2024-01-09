#include "utils/vulkan/device.hpp"

#include "vulkan/vulkan.h"


namespace utils::vulkan {

    utils::Logger Device::log = Logger("Device");


    Device::Device(
        std::shared_ptr<InstanceHandle> const& vkInstanceHandle,
        VkPhysicalDevice const& physicalDevice,
        uint32_t const queueFamilyIndex,
        uint32_t const queueCount,
        std::vector<std::string> const& validationLayerNames
    ) :
        vkInstanceHandle(vkInstanceHandle),
        vkDeviceHandle(std::make_shared<DeviceHandle>())
    {
        INFO(log) << "Creating logical device." << std::endl;

        float queuePriority = 1.0f;

        VkDeviceQueueCreateInfo queueCreateInfo {};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
        queueCreateInfo.queueCount = queueCount;
        queueCreateInfo.pQueuePriorities = &queuePriority;

        VkPhysicalDeviceFeatures deviceFeatures {};

        auto const validationLayerParams = StringParameters(validationLayerNames);

        VkDeviceCreateInfo createInfo {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.pQueueCreateInfos = &queueCreateInfo;
        createInfo.queueCreateInfoCount = 1;
        createInfo.pEnabledFeatures = &deviceFeatures;
        createInfo.enabledLayerCount = validationLayerParams.size();
        createInfo.ppEnabledLayerNames = validationLayerParams.data();

        if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &this->vkDeviceHandle->vk) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create logical device.");
        }
    }

}
