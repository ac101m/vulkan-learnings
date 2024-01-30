#pragma once

#include "utils/misc/logging.hpp"
#include "utils/vulkan/handles.hpp"
#include "utils/vulkan/render_pass.hpp"
#include "utils/vulkan/frame_buffer.hpp"
#include "utils/vulkan/graphics_pipeline.hpp"
#include "utils/vulkan/buffer.hpp"
#include "utils/vulkan/descriptor_set.hpp"
#include "utils/vulkan/pipeline_layout.hpp"


namespace utils::vulkan {

    class CommandBuffer {
    private:
        static utils::Logger log;

        std::shared_ptr<DeviceHandle> const vkDeviceHandle;
        std::shared_ptr<CommandPoolHandle> const vkCommandPoolHandle;

    public:
        VkCommandBuffer_T * vk;

    public:
        CommandBuffer(
            std::shared_ptr<DeviceHandle> const& vkDeviceHandle,
            std::shared_ptr<CommandPoolHandle> const& vkCommandPoolHandle,
            VkCommandBufferLevel const bufferLevel);

        /**
         * @brief Begin writing to the command buffer.
         * @param flags Command buffer usage flags (defaults to none).
         */
        void begin(VkCommandBufferUsageFlagBits const flags = static_cast<VkCommandBufferUsageFlagBits>(0));

        /**
         * @brief Finalize the command buffer.
         */
        void end();

        /**
         * @brief Reset the command buffer so that it can be re-recorded.
         */
        void reset();

        /**
         * @brief Begin render pass.
         * @param renderPass Shared pointer to render pass object.
         * @param frameBuffer Shared pointer to frame buffer for rendering.
         * @param renderOffset Offset to begin rendering at.
         * @param renderExtent Maximum extent of rendering.
         */
        void beginRenderPass(
            std::shared_ptr<RenderPass> const& renderPass,
            std::shared_ptr<FrameBuffer> const& frameBuffer,
            VkOffset2D const renderOffset,
            VkExtent2D const renderExtent);

        /**
         * @brief End render pass.
         */
        void endRenderPass();

        /**
         * @brief Bind graphics pipeline
         * @param graphicsPipeline Shared pointer to graphics pipeline object.
         */
        void bindGraphicsPipeline(std::shared_ptr<GraphicsPipeline> const& graphicsPipeline);

        /**
         * @brief Set viewport settings.
         * @param renderExtent Extent of the viewport.
         */
        void setViewport(VkExtent2D const renderExtent);

        /**
         * @brief Set scissor settings.
         * @param scissorOffset Offset of the scissor operation.
         * @param scissorExtent Extent of the scissor operation.
         */
        void setScissor(VkOffset2D const scissorOffset, VkExtent2D const scissorExtent);

        /**
         * @brief Draw some stuff!
         * @param vertexCount Number of vertices to render.
         * @param instanceCount Number of instances to render.
         * @param firstVertex Offset into the vertex buffer.
         * @param firstInstance Offset into the instance buffer.
         */
        void draw(
            uint32_t const vertexCount,
            uint32_t const instanceCount,
            uint32_t const firstVertex,
            uint32_t const firstInstance);

        /**
         * @brief Draw some stuff! (indexed)
         * @param indexCount The number of vertices to draw.
         * @param instanceCount The number of instances to draw.
         * @param firstIndex Base index within the index buffer.
         * @param vertexOffset An offset into the vertex buffer, added to each index.
         * @param firstInstance
         */
        void drawIndexed(
            uint32_t const indexCount,
            uint32_t const instanceCount,
            uint32_t const firstIndex,
            int32_t const vertexOffset,
            uint32_t const firstInstance);

        /**
         * @brief Bind a descriptor set.
         * @param descriptorSet Shared pointer to descriptor set to bind.
         * @param pipelineLayout Shared pointer to pipeline layout object.
         * @param bindPoint Bind point for the (e.g. VK_PIPELINE_BIND_POINT_GRAPHICS)
         */
        void bindDescriptorSet(
            std::shared_ptr<DescriptorSet> const& descriptorSet,
            std::shared_ptr<PipelineLayout> const& pipelineLayout,
            VkPipelineBindPoint const bindPoint);

        /**
         * @brief Bind a single vertex buffer.
         * @param vertexBuffer Pointer to vertex buffer.
         */
        void bindVertexBuffer(std::shared_ptr<Buffer> const& vertexBuffer);

        /**
         * @brief Bind an index buffer.
         * @param indexBuffer Shared pointer to index buffer.
         * @param indexType The datatype of the indices.
         */
        void bindIndexBuffer(std::shared_ptr<Buffer> const& indexBuffer, VkIndexType const indexType);

        /**
         * @brief Copy a subsection of one buffer to a subsection of another buffer.
         * @param sourceBuffer Shared pointer to the source buffer.
         * @param destinationBuffer Shared pointer to destination buffer.
         * @param sourceOffset Offset to start copying from the source buffer.
         * @param destinationOffset Offset to start copying to the destination buffer.
         * @param size Number of bytes to copy.
         */
        void copyBuffer(
            std::shared_ptr<Buffer> const& sourceBuffer,
            std::shared_ptr<Buffer> const& destinationBuffer,
            uint64_t const sourceOffset,
            uint64_t const destinationOffset,
            uint64_t const size);

        /**
         * @brief Copy the contents of one buffer to another buffer of equal size.
         * @param sourceBuffer Shared pointer to the source buffer.
         * @param destinationBuffer Shared pointer to destination buffer.
         */
        void copyBuffer(
            std::shared_ptr<Buffer> const& sourceBuffer,
            std::shared_ptr<Buffer> const& destinationBuffer);
    };

}
