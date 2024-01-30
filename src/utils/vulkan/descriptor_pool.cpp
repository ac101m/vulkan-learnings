#include "utils/vulkan/descriptor_pool.hpp"


namespace utils::vulkan {

    utils::Logger DescriptorPool::log("DescriptorPool");


    DescriptorPool::DescriptorPool(
        std::shared_ptr<DeviceHandle> const& vkDeviceHandle,
        DescriptorPoolConfig const& config
    ) :
        HandleWrapper<DescriptorPoolHandle>(std::make_shared<DescriptorPoolHandle>(vkDeviceHandle)),
        vkDeviceHandle(vkDeviceHandle)
    {
        INFO(log) << "Creating descriptor pool." << std::endl;

        VkDescriptorPoolCreateInfo createInfo {};
        createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        createInfo.flags = 0;
        createInfo.maxSets = config.maxSets;
        createInfo.poolSizeCount = static_cast<uint32_t>(config.poolSizes.size());
        createInfo.pPoolSizes = config.poolSizes.data();

        if (vkCreateDescriptorPool(this->vkDeviceHandle->vk, &createInfo, nullptr, &this->vkHandle->vk) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create descriptor pool.");
        }
    }


    std::shared_ptr<DescriptorSet> DescriptorPool::allocateDescriptorSet(std::shared_ptr<DescriptorSetLayout> const& layout) {
        return std::make_shared<DescriptorSet>(this->vkDeviceHandle, this->vkHandle, layout->getHandle());
    }

}
