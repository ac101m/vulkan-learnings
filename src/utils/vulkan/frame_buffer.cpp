#include "utils/vulkan/frame_buffer.hpp"


namespace utils::vulkan {

    utils::Logger FrameBuffer::log("FrameBuffer");


    FrameBuffer::FrameBuffer(
        std::shared_ptr<DeviceHandle> const& vkDeviceHandle,
        std::shared_ptr<RenderPassHandle> const& vkRenderPassHandle,
        FrameBufferConfig const& config
    ) :
        HandleWrapper<FrameBufferHandle>(std::make_shared<FrameBufferHandle>(vkDeviceHandle)),
        vkDeviceHandle(vkDeviceHandle),
        vkRenderPassHandle(vkRenderPassHandle),
        vkAttachments(config.attachments)
    {
        INFO(log) << "Creating frame buffer." << std::endl;

        VkImageView attachments[config.attachments.size()];
        for (unsigned i = 0; i < config.attachments.size(); i++) {
            attachments[i] = config.attachments[i]->vk;
        }

        VkFramebufferCreateInfo createInfo {};
        createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        createInfo.renderPass = this->vkRenderPassHandle->vk;
        createInfo.attachmentCount = static_cast<uint32_t>(config.attachments.size());
        createInfo.pAttachments = attachments;
        createInfo.width = config.size.width;
        createInfo.height = config.size.height;
        createInfo.layers = config.layerCount;

        if (vkCreateFramebuffer(this->vkDeviceHandle->vk, &createInfo, nullptr, &this->vkHandle->vk) != VK_SUCCESS) {
            throw std::runtime_error("failed to create framebuffer.");
        }
    }

}
