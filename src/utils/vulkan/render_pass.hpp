#pragma once

#include "utils/misc/logging.hpp"
#include "utils/vulkan/handles.hpp"

#include <memory>
#include <vector>


namespace utils::vulkan {

    /**
     * @brief Helper class for configuring render pass attachments.
     */
    struct AttachmentDescription {
        VkFormat format = VK_FORMAT_UNDEFINED;
        VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT;
        VkAttachmentLoadOp loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        VkAttachmentStoreOp storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        VkAttachmentLoadOp stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        VkAttachmentStoreOp stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        VkImageLayout initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        VkImageLayout finalLayout = VK_IMAGE_LAYOUT_UNDEFINED;

        VkAttachmentDescription toVulkan() const {
            VkAttachmentDescription vk {};
            vk.format = this->format;
            vk.samples = this->samples;
            vk.loadOp = this->loadOp;
            vk.storeOp = this->storeOp;
            vk.stencilLoadOp = this->stencilLoadOp;
            vk.stencilStoreOp = this->stencilStoreOp;
            vk.initialLayout = this->initialLayout;
            vk.finalLayout = this->finalLayout;
            return vk;
        }
    };


    /**
     * @brief Helper class for configuring render pass subpasses.
     */
    struct SubPassDescription {
        std::vector<VkAttachmentReference> colorAttachments;
        VkPipelineBindPoint pipelineBindPoint;

        SubPassDescription(VkPipelineBindPoint const pipelineBindPoint) {
            this->pipelineBindPoint = pipelineBindPoint;
        }

        VkSubpassDescription toVulkan() const {
            VkSubpassDescription vk {};
            vk.pipelineBindPoint = this->pipelineBindPoint;
            vk.colorAttachmentCount = static_cast<uint32_t>(this->colorAttachments.size());
            vk.pColorAttachments = this->colorAttachments.data();
            return vk;
        }

        void addColorAttachment(uint32_t const attachmentIndex, VkImageLayout const imageLayout) {
            VkAttachmentReference vk {};
            vk.attachment = attachmentIndex;
            vk.layout = imageLayout;
            colorAttachments.push_back(vk);
        }
    };


    /**
     * @brief Helper class for configuring render passes.
     */
    struct RenderPassConfig {
        std::vector<AttachmentDescription> attachments;
        std::vector<SubPassDescription> subPasses;

        /**
         * @brief Add an attachment description.
         * @param description Attachment description to add.
         * @return Index of the newly added attachment.
         */
        uint32_t addAttachment(AttachmentDescription const& description) {
            attachments.push_back(description);
            return attachments.size() - 1;
        }

        /**
         * @brief Add a sub-pass description.
         * @param description Sub pass description to add.
         */
        void addSubPass(SubPassDescription const& description) {
            subPasses.push_back(description);
        }
    };


    class RenderPass : public HandleWrapper<RenderPassHandle> {
    private:
        static utils::Logger log;

        std::shared_ptr<DeviceHandle> const vkDeviceHandle;

        RenderPassConfig const config;

    public:
        RenderPass(std::shared_ptr<DeviceHandle> const& vkDeviceHandle, RenderPassConfig const& config);
    };

}
