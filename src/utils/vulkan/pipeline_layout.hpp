#pragma once

#include "utils/misc/logging.hpp"
#include "utils/vulkan/handles.hpp"
#include "utils/vulkan/helpers.hpp"

#include <memory>


namespace utils::vulkan {

    struct PipelineLayoutConfig {
        // TODO
    };


    class PipelineLayout : public HandleWrapper<PipelineLayoutHandle> {
    private:
        static utils::Logger log;

        std::shared_ptr<DeviceHandle> const vkDeviceHandle;

        PipelineLayoutConfig const config;

    public:
        PipelineLayout(std::shared_ptr<DeviceHandle> const& vkDeviceHandle, PipelineLayoutConfig const& config);
    };

}
