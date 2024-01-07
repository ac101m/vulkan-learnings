#include "utils/vulkan/device.hpp"

#include "vulkan/vulkan.h"


namespace utils::vulkan {

    Device::Device(PhysicalDevice const& physicalDevice) {

    }


    Device::~Device() {
        //vkDestroyDevice(this->vkDevice, nullptr);
    }

}
