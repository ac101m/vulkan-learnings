#include "utils/vulkan/pipeline_layout.hpp"


namespace utils::vulkan {

    utils::Logger PipelineLayout::log("PipelineLayout");


    PipelineLayout::PipelineLayout(
        std::shared_ptr<DeviceHandle> const& vkDeviceHandle,
        PipelineLayoutConfig const& config
    ) :
        HandleWrapper<PipelineLayoutHandle>(std::make_shared<PipelineLayoutHandle>(vkDeviceHandle)),
        vkDeviceHandle(vkDeviceHandle),
        config(config)
    {
        INFO(log) << "Creating pipeline layout." << std::endl;

        VkDescriptorSetLayout descriptorSets[config.descriptorSets.size()];
        for (unsigned i = 0; i < config.descriptorSets.size(); i++) {
            descriptorSets[i] = config.descriptorSets[i]->getHandle()->vk;
        }

        VkPipelineLayoutCreateInfo pipelineLayoutInfo {};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(config.descriptorSets.size());
        pipelineLayoutInfo.pSetLayouts = descriptorSets;
        pipelineLayoutInfo.pushConstantRangeCount = 0;      // Optional/Todo
        pipelineLayoutInfo.pPushConstantRanges = nullptr;   // Optional/Todo

        if (vkCreatePipelineLayout(this->vkDeviceHandle->vk, &pipelineLayoutInfo, nullptr, &this->vkHandle->vk) != VK_SUCCESS) {
            throw std::runtime_error("failed to create pipeline layout.");
        }
    }

}
