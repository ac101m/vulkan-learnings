#pragma once

#include "utils/misc/logging.hpp"
#include "utils/vulkan/handles.hpp"


namespace utils::vulkan {

    class CommandBuffer {
    private:
        static utils::Logger log;

        VkCommandBuffer_T * vk;

        std::shared_ptr<DeviceHandle> const vkDeviceHandle;
        std::shared_ptr<CommandPoolHandle> const vkCommandPoolHandle;

    public:
        CommandBuffer(
            std::shared_ptr<DeviceHandle> const& vkDeviceHandle,
            std::shared_ptr<CommandPoolHandle> const& vkCommandPoolHandle,
            VkCommandBufferLevel const bufferLevel);

        /**
         * @brief Begin writing to the command buffer.
         * @param flags Command buffer usage flags (defaults to none).
         */
        void beginCommandBuffer(VkCommandBufferUsageFlagBits const flags = static_cast<VkCommandBufferUsageFlagBits>(0));
    };

}
