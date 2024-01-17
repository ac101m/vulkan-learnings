#pragma once

#include "utils/misc/logging.hpp"
#include "utils/vulkan/handles.hpp"


namespace utils::vulkan {

    struct CommandBufferConfig {
        VkCommandBufferLevel bufferLevel = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    };


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
            CommandBufferConfig const& config);
    };

}
