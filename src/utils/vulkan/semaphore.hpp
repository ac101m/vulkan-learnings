#pragma once

#include "utils/misc/logging.hpp"
#include "utils/vulkan/handles.hpp"


namespace utils::vulkan {

    class Semaphore : public HandleWrapper<SemaphoreHandle> {
    private:
        static utils::Logger log;

        std::shared_ptr<DeviceHandle> const vkDeviceHandle;

    public:
        Semaphore(std::shared_ptr<DeviceHandle> const& vkDeviceHandle);
    };

}
