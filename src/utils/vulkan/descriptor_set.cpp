#include "utils/vulkan/descriptor_set.hpp"


namespace utils::vulkan {

    utils::Logger DescriptorSet::log("DescriptorSet");


    DescriptorSet::DescriptorSet(
        std::shared_ptr<DeviceHandle> const& vkDeviceHandle,
        std::shared_ptr<DescriptorPoolHandle> const& vkDescriptorPoolHandle,
        std::shared_ptr<DescriptorSetLayoutHandle> const& vkDescriptorSetLayoutHandle
    ) :
        vkDeviceHandle(vkDeviceHandle),
        vkDescriptorPoolHandle(vkDescriptorPoolHandle),
        vkDescriptorSetLayoutHandle(vkDescriptorSetLayoutHandle)
    {
        INFO(log) << "Allocating descriptor set." << std::endl;

        VkDescriptorSetAllocateInfo allocateInfo {};
        allocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocateInfo.descriptorPool = vkDescriptorPoolHandle->vk;
        allocateInfo.descriptorSetCount = 1;
        allocateInfo.pSetLayouts = &vkDescriptorSetLayoutHandle->vk;

        if (vkAllocateDescriptorSets(this->vkDeviceHandle->vk, &allocateInfo, &this->vk)) {
            throw std::runtime_error("failed to allocate descriptor set.");
        }
    }


    void DescriptorSet::update(std::shared_ptr<Buffer> const& buffer) {
        VkDescriptorBufferInfo bufferInfo {};
        bufferInfo.buffer = buffer->getHandle()->vk;
        bufferInfo.offset = 0;
        bufferInfo.range = VK_WHOLE_SIZE;

        VkWriteDescriptorSet descriptorWrite {};
        descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrite.dstSet = this->vk;
        descriptorWrite.dstBinding = 0;
        descriptorWrite.dstArrayElement = 0;
        descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrite.descriptorCount = 1;
        descriptorWrite.pBufferInfo = &bufferInfo;
        descriptorWrite.pImageInfo = nullptr;
        descriptorWrite.pTexelBufferView = nullptr;

        vkUpdateDescriptorSets(this->vkDeviceHandle->vk, 1, &descriptorWrite, 0, nullptr);
    }

}
