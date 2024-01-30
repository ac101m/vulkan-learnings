#include "utils/vulkan/descriptor_set_layout.hpp"


namespace utils::vulkan {

    DescriptorSetLayout::DescriptorSetLayout(
        std::shared_ptr<DeviceHandle> const& vkDeviceHandle,
        DescriptorSetLayoutConfig const& config
    ) :
        HandleWrapper<DescriptorSetLayoutHandle>(std::make_shared<DescriptorSetLayoutHandle>(vkDeviceHandle)),
        vkDeviceHandle(vkDeviceHandle)
    {
        VkDescriptorSetLayoutCreateInfo createInfo {};
        createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        createInfo.bindingCount = static_cast<uint32_t>(config.descriptorBindings.size());
        createInfo.pBindings = config.descriptorBindings.data();

        if (vkCreateDescriptorSetLayout(this->vkDeviceHandle->vk, &createInfo, nullptr, &this->vkHandle->vk) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create descriptor set layout.");
        }
    }

}
