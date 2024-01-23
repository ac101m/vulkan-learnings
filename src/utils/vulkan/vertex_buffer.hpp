#pragma once

#include "utils/misc/logging.hpp"
#include "utils/vulkan/handles.hpp"

#include <memory>


namespace utils::vulkan {

    class VertexBuffer : public HandleWrapper<BufferHandle> {
    private:
        static utils::Logger log;

        std::shared_ptr<DeviceHandle> const vkDeviceHandle;

    public:
        VertexBuffer(
            std::shared_ptr<DeviceHandle> const& vkDeviceHandle,
            uint64_t const size,
            VkSharingMode const sharingMode);

        /**
         * @brief Get the memory requirements of the buffer.
         * @return VkMemoryRequirements structure containing memory requirements info.
         */
        VkMemoryRequirements getMemoryRequirements() const;
    };

}
