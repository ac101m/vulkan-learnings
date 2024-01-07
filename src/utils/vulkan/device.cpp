#include "utils/vulkan/device.hpp"

#include "vulkan/vulkan.h"


namespace utils::vulkan {

    Device::Device(PhysicalDevice& physicalDevice, QueueFamily const& queueFamily) {
        float queuePriority = 1.0f;

        VkDeviceQueueCreateInfo queueCreateInfo {};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily.index;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;

        VkPhysicalDeviceFeatures deviceFeatures {};

        VkDeviceCreateInfo createInfo {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.pQueueCreateInfos = &queueCreateInfo;
        createInfo.queueCreateInfoCount = 1;
        createInfo.pEnabledFeatures = &deviceFeatures;
        createInfo.enabledExtensionCount = 0;

        if (vkCreateDevice(physicalDevice.get(), &createInfo, nullptr, &this->vkDevice) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create logical device.");
        }
    }


    Device::~Device() {
        vkDestroyDevice(this->vkDevice, nullptr);
    }

}
