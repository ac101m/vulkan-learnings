#pragma once

#include "utils/misc/logging.hpp"
#include "utils/vulkan/handles.hpp"
#include "utils/vulkan/render_pass.hpp"
#include "utils/vulkan/frame_buffer.hpp"
#include "utils/vulkan/graphics_pipeline.hpp"


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
        void draw(uint32_t const vertexCount, uint32_t const instanceCount, uint32_t const firstVertex, uint32_t const firstInstance);
    };

}
