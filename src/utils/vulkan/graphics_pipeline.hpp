#pragma once

#include "utils/misc/logging.hpp"
#include "utils/vulkan/handles.hpp"


namespace utils::vulkan {

    /**
     * @brief Helper object for shader stage configuration.
     */
    struct ShaderStageConfig {
        std::shared_ptr<ShaderModuleHandle> shaderModule;
        VkShaderStageFlagBits flagBits;
        std::string const shaderMain;

        /**
         * @brief Convert the config to vulkan create info object.
         * @return Pipeline shader stage create info object.
         */
        VkPipelineShaderStageCreateInfo toVulkan() const {
            VkPipelineShaderStageCreateInfo vk {};
            vk.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            vk.stage = this->flagBits;
            vk.module = this->shaderModule->vk;
            vk.pName = this->shaderMain.c_str();
            return vk;
        }
    };


    /**
     * @brief Helper object for configuring graphics pipeline.
     */
    struct GraphicsPipelineConfig {
        std::vector<ShaderStageConfig> shaderStages;

        /**
         * @brief Add a shader stage to the configuration.
         * @param shaderModule Shared pointer to shader module handle.
         * @param shaderFlags Shader stage flag bits.
         * @param shaderMain String containing name of the main function in the shader.
         */
        void addShaderStage(
            std::shared_ptr<ShaderModuleHandle> const& shaderModule,
            VkShaderStageFlagBits const shaderFlags,
            std::string const& shaderMain = "main"
        ) {
            shaderStages.push_back({shaderModule, shaderFlags, shaderMain});
        }
    };


    class GraphicsPipeline : public HandleWrapper<PipelineHandle> {
    private:
        static utils::Logger log;

        std::shared_ptr<DeviceHandle> const vkDeviceHandle;
        std::shared_ptr<PipelineLayoutHandle> const vkPipelineLayoutHandle;
        std::shared_ptr<RenderPassHandle> const vkRenderPassHandle;

        GraphicsPipelineConfig const config;

    public:
        GraphicsPipeline(
            std::shared_ptr<DeviceHandle> const& vkDeviceHandle,
            std::shared_ptr<PipelineLayoutHandle> const& vkPipelineLayoutHandle,
            std::shared_ptr<RenderPassHandle> const& vkRenderPassHandle,
            GraphicsPipelineConfig const& config);

    };

}
