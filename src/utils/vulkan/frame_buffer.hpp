#pragma once

#include "utils/misc/logging.hpp"
#include "utils/vulkan/handles.hpp"

#include <memory>


namespace utils::vulkan {

    /**
     * @brief Utility struct for constructing.
     */
    struct FrameBufferConfig {
        std::vector<std::shared_ptr<ImageViewHandle>> attachments;
        VkExtent2D const size;
        unsigned layerCount = 1;

        FrameBufferConfig(VkExtent2D const size) : size(size) {}

        void addAttachment(std::shared_ptr<ImageViewHandle> const& imageViewHandle) {
            this->attachments.push_back(imageViewHandle);
        }
    };


    class FrameBuffer : public HandleWrapper<FrameBufferHandle> {
    private:
        static utils::Logger log;

        std::shared_ptr<DeviceHandle> const vkDeviceHandle;
        std::shared_ptr<RenderPassHandle> const vkRenderPassHandle;

        std::vector<std::shared_ptr<ImageViewHandle>> const vkAttachments;

    public:
        FrameBuffer(
            std::shared_ptr<DeviceHandle> const& vkDeviceHandle,
            std::shared_ptr<RenderPassHandle> const& vkRenderPassHandle,
            FrameBufferConfig const& config);
    };

}
