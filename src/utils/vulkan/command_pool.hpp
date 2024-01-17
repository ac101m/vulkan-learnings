#pragma once

#include "utils/misc/logging.hpp"
#include "utils/vulkan/handles.hpp"
#include "utils/vulkan/command_buffer.hpp"


namespace utils::vulkan {

    struct CommandPoolConfig {
        VkCommandPoolCreateFlagBits flagBits = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        uint32_t queueFamilyIndex;

        CommandPoolConfig(uint32_t const queueFamilyIndex) {
            this->queueFamilyIndex = queueFamilyIndex;
        }
    };


    class CommandPool : HandleWrapper<CommandPoolHandle> {
    private:
        static utils::Logger log;

        std::shared_ptr<DeviceHandle> const vkDeviceHandle;

    public:
        CommandPool(std::shared_ptr<DeviceHandle> const& vkDeviceHandle, CommandPoolConfig const& config);

        /**
         * @brief Create a new command buffer.
         * @param config Command buffer config object.
         * @return Shared pointer to command buffer object.
         */
        std::shared_ptr<CommandBuffer> createCommandBuffer(CommandBufferConfig const& config) const;
    };

}
