#pragma once

#include "utils/misc/logging.hpp"
#include "utils/vulkan/handles.hpp"

#include <string>
#include <memory>
#include <filesystem>


namespace utils::vulkan {

    class ShaderModule {
    private:
        static utils::Logger log;

        std::shared_ptr<ShaderModuleHandle> const vkShaderModuleHandle;
        std::shared_ptr<DeviceHandle> const vkDeviceHandle;

        std::filesystem::path const path;

    public:
        ShaderModule(
            std::shared_ptr<DeviceHandle> const& vkDeviceHandle,
            std::filesystem::path const& path);
    };

}
