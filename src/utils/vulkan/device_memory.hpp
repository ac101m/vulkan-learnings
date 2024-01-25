#pragma once

#include "utils/vulkan/handles.hpp"
#include "utils/misc/logging.hpp"

#include <memory>


namespace utils::vulkan {

    class DeviceMemory : public HandleWrapper<DeviceMemoryHandle> {
    private:
        static utils::Logger log;

        std::shared_ptr<DeviceHandle> const vkDeviceHandle;

    public:
        uint32_t const type;
        uint64_t const size;

    public:
        DeviceMemory(
            std::shared_ptr<DeviceHandle> const& vkDeviceHandle,
            uint32_t const memoryTypeIndex,
            uint64_t const allocationSize);
    };

}
