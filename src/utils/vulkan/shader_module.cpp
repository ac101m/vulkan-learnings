#include "utils/vulkan/shader_module.hpp"
#include "utils/misc/file.hpp"


namespace utils::vulkan {

    utils::Logger ShaderModule::log("ShaderModule");


    ShaderModule::ShaderModule(
        std::shared_ptr<DeviceHandle> const& vkDeviceHandle,
        std::filesystem::path const& path
    ) :
        vkShaderModuleHandle(std::make_shared<ShaderModuleHandle>(vkDeviceHandle)),
        vkDeviceHandle(vkDeviceHandle),
        path(path)
    {
        INFO(log) << "Loading shader module from " << path << std::endl;

        auto const shaderCode = utils::readBinaryFile(path);

        VkShaderModuleCreateInfo createInfo {};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = shaderCode.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(shaderCode.data());

        if (vkCreateShaderModule(this->vkDeviceHandle->vk, &createInfo, nullptr, &this->vkShaderModuleHandle->vk) != VK_SUCCESS) {
            throw std::runtime_error("failed to create shader module.");
        }
    }

}
