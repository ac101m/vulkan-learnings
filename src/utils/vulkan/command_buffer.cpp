#include "utils/vulkan/command_buffer.hpp"


namespace utils::vulkan {

    utils::Logger CommandBuffer::log("CommandBuffer");


    CommandBuffer::CommandBuffer(
        std::shared_ptr<DeviceHandle> const& vkDeviceHandle,
        std::shared_ptr<CommandPoolHandle> const& vkCommandPoolHandle,
        VkCommandBufferLevel const bufferLevel
    ) : vkDeviceHandle(vkDeviceHandle), vkCommandPoolHandle(vkCommandPoolHandle) {
        INFO(log) << "Creating command buffer." << std::endl;

        VkCommandBufferAllocateInfo createInfo {};
        createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        createInfo.commandPool = vkCommandPoolHandle->vk;
        createInfo.level = bufferLevel;
        createInfo.commandBufferCount = 1;

        if (vkAllocateCommandBuffers(this->vkDeviceHandle->vk, &createInfo, &this->vk) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate command buffer.");
        }
    }


    void CommandBuffer::begin(VkCommandBufferUsageFlagBits const flags) {
        VkCommandBufferBeginInfo beginInfo {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = flags;
        beginInfo.pInheritanceInfo = nullptr; // Optional

        if (vkBeginCommandBuffer(this->vk, &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("failed to begin recording command buffer.");
        }
    }


    void CommandBuffer::end() {
        if (vkEndCommandBuffer(this->vk) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer.");
        }
    }


    void CommandBuffer::reset() {
        vkResetCommandBuffer(this->vk, 0);
    }


    void CommandBuffer::beginRenderPass(
        std::shared_ptr<RenderPass> const& renderPass,
        std::shared_ptr<FrameBuffer> const& frameBuffer,
        VkOffset2D const renderOffset,
        VkExtent2D const renderExtent
    ) {
        VkClearValue clearValues = {{{0.0f, 0.0f, 0.0f, 1.0f}}};

        VkRenderPassBeginInfo renderPassInfo {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = renderPass->getHandle()->vk;
        renderPassInfo.framebuffer = frameBuffer->getHandle()->vk;
        renderPassInfo.renderArea.offset = renderOffset;
        renderPassInfo.renderArea.extent = renderExtent;
        renderPassInfo.pClearValues = &clearValues;
        renderPassInfo.clearValueCount = 1;

        vkCmdBeginRenderPass(this->vk, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    }


    void CommandBuffer::endRenderPass() {
        vkCmdEndRenderPass(this->vk);
    }


    void CommandBuffer::bindGraphicsPipeline(std::shared_ptr<GraphicsPipeline> const& graphicsPipeline) {
        vkCmdBindPipeline(this->vk, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline->getHandle()->vk);
    }


    void CommandBuffer::setViewport(VkExtent2D const renderExtent) {
        VkViewport viewPort {};
        viewPort.x = 0.0f;
        viewPort.y = 0.0f;
        viewPort.width = static_cast<float>(renderExtent.width);
        viewPort.height = static_cast<float>(renderExtent.height);
        viewPort.minDepth = 0.0f;
        viewPort.maxDepth = 1.0f;
        vkCmdSetViewport(this->vk, 0, 1, &viewPort);
    }


    void CommandBuffer::setScissor(VkOffset2D const scissorOffset, VkExtent2D const scissorExtent) {
        VkRect2D scissor {};
        scissor.offset = scissorOffset;
        scissor.extent = scissorExtent;
        vkCmdSetScissor(this->vk, 0, 1, &scissor);
    }


    void CommandBuffer::draw(
        uint32_t const vertexCount,
        uint32_t const instanceCount,
        uint32_t const firstVertex,
        uint32_t const firstInstance
    ) {
        vkCmdDraw(this->vk, vertexCount, instanceCount, firstVertex, firstInstance);
    }


    void CommandBuffer::drawIndexed(
        uint32_t const indexCount,
        uint32_t const instanceCount,
        uint32_t const firstIndex,
        int32_t const vertexOffset,
        uint32_t const firstInstance
    ) {
        vkCmdDrawIndexed(this->vk, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
    }


    void CommandBuffer::bindDescriptorSet(
        std::shared_ptr<DescriptorSet> const& descriptorSet,
        std::shared_ptr<PipelineLayout> const& pipelineLayout,
        VkPipelineBindPoint const bindPoint
    ) {
        vkCmdBindDescriptorSets(this->vk, bindPoint, pipelineLayout->getHandle()->vk, 0, 1, &descriptorSet->vk, 0, nullptr);
    }


    void CommandBuffer::bindVertexBuffer(std::shared_ptr<Buffer> const& vertexBuffer) {
        VkBuffer vertexBuffers[] = {vertexBuffer->getHandle()->vk};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(this->vk, 0, 1, vertexBuffers, offsets);
    }


    void CommandBuffer::bindIndexBuffer(std::shared_ptr<Buffer> const& indexBuffer, VkIndexType const indexType) {
        vkCmdBindIndexBuffer(this->vk, indexBuffer->getHandle()->vk, 0, indexType);
    }


    void CommandBuffer::copyBuffer(
        std::shared_ptr<Buffer> const& sourceBuffer,
        std::shared_ptr<Buffer> const& destinationBuffer,
        uint64_t const sourceOffset,
        uint64_t const destinationOffset,
        uint64_t const size
    ) {
        VkBufferCopy copyRegion {};
        copyRegion.srcOffset = sourceOffset;
        copyRegion.dstOffset = destinationOffset;
        copyRegion.size = size;

        vkCmdCopyBuffer(this->vk, sourceBuffer->getHandle()->vk, destinationBuffer->getHandle()->vk, 1, &copyRegion);
    }


    void CommandBuffer::copyBuffer(
        std::shared_ptr<Buffer> const& sourceBuffer,
        std::shared_ptr<Buffer> const& destinationBuffer
    ) {
        if (sourceBuffer->getMemorySize() != destinationBuffer->getMemorySize()) {
            throw std::runtime_error("Unable to perform full buffer copy, source and destination buffers differ in size.");
        }

        this->copyBuffer(sourceBuffer, destinationBuffer, 0, 0, sourceBuffer->getMemorySize());
    }


    void CommandBuffer::pipelineBarrier(VkImageMemoryBarrier const& barrier) {
        vkCmdPipelineBarrier(
            this->vk,
            0, // TODO
            0, // TODO
            0,
            0, nullptr,
            0, nullptr,
            1, &barrier
        );
    }
}
