#include "utils/vulkan/render_pass.hpp"


namespace utils::vulkan {

    utils::Logger RenderPass::log("RenderPass");


    RenderPass::RenderPass(
        std::shared_ptr<DeviceHandle> const& vkDeviceHandle, RenderPassConfig const& config
    ) :
        HandleWrapper<RenderPassHandle>(std::make_shared<RenderPassHandle>(vkDeviceHandle)),
        vkDeviceHandle(vkDeviceHandle),
        config(config)
    {
        VkAttachmentDescription attachmentDescriptions[config.attachments.size()];
        for (unsigned i = 0; i < config.attachments.size(); i++) {
            attachmentDescriptions[i] = config.attachments[i].toVulkan();
        }

        VkSubpassDescription subpassDescriptions[config.subPasses.size()];
        for (unsigned i = 0; i < config.subPasses.size(); i++) {
            subpassDescriptions[i] = config.subPasses[i].toVulkan();
        }

        VkRenderPassCreateInfo renderPassInfo {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = static_cast<uint32_t>(config.attachments.size());
        renderPassInfo.pAttachments = attachmentDescriptions;
        renderPassInfo.subpassCount = static_cast<uint32_t>(config.subPasses.size());
        renderPassInfo.pSubpasses = subpassDescriptions;

        if (vkCreateRenderPass(this->vkDeviceHandle->vk, &renderPassInfo, nullptr, &this->vkHandle->vk) != VK_SUCCESS) {
            throw std::runtime_error("failed to create render pass.");
        }
    }

}
