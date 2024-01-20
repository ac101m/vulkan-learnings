#pragma once

#include "utils/misc/logging.hpp"
#include "utils/vulkan/handles.hpp"


namespace utils::vulkan {

    class Fence : public HandleWrapper<FenceHandle> {
    private:
        static utils::Logger log;

        std::shared_ptr<DeviceHandle> const vkDeviceHandle;

    public:
        Fence(std::shared_ptr<DeviceHandle> const& vkDeviceHandle, VkFenceCreateFlagBits const flags);

        /**
         * @brief Wait for the fence.
         */
        void wait();

        /**
         * @brief Reset the fence.
         */
        void reset();
    };

}
