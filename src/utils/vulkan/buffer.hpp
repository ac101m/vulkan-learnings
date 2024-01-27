#pragma once

#include "utils/misc/logging.hpp"
#include "utils/vulkan/handles.hpp"
#include "utils/vulkan/device_memory.hpp"

#include <memory>


namespace utils::vulkan {

    class Buffer : public HandleWrapper<BufferHandle> {
    private:
        static utils::Logger log;

        std::shared_ptr<DeviceHandle> const vkDeviceHandle;

        std::shared_ptr<DeviceMemoryHandle> vkDeviceMemory = nullptr;

        uint64_t memoryOffset = 0;
        uint64_t memorySize = 0;

        void * data = nullptr;

    public:
        Buffer(
            std::shared_ptr<DeviceHandle> const& vkDeviceHandle,
            uint64_t const size,
            VkBufferUsageFlags const usageFlags,
            VkSharingMode const sharingMode);

        /**
         * @brief Get the memory requirements of the buffer.
         * @return VkMemoryRequirements structure containing memory requirements info.
         */
        VkMemoryRequirements getMemoryRequirements() const;

        /**
         * @brief Bind device memory.
         * @param deviceMemory Shared pointer to device memory object.
         * @param offset Offset within the device memory block in bytes.
         */
        void bindMemory(std::shared_ptr<DeviceMemory> const& deviceMemory, uint64_t const offset);

        /**
         * @brief Maps buffer into host memory and sets the data pointer to point at the mapped memory.
         */
        void mapMemory();

        /**
         * @brief Unmaps buffer from host memory and resets the data pointer to null.
         */
        void unmapMemory();

        /**
         * @brief Retrieves the data pointer.
         */
        void * getMappedMemory() {
            return this->data;
        }

        /**
         * @brief Get the size of the buffer memory in bytes.
         */
        uint64_t getMemorySize() {
            return this->memorySize;
        }
    };

}
