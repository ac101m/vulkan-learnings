#pragma once

#include "utils/misc/logging.hpp"
#include "utils/vulkan/handles.hpp"


namespace utils::vulkan {

    /**
     * @brief Helper object for passing vertex binding and attribute descriptions.
     */
    struct VertexInfo {
        std::vector<VkVertexInputBindingDescription> vertexTypes;
        std::vector<VkVertexInputAttributeDescription> attributes;

        /**
         * @brief Add a vertex adescription to the vertex info.
         * @param size Size of the vertex data in bytes.
         * @param inputRate Either VK_VERTEX_INPUT_RATE_VERTEX or VK_VERTEX_INPUT_RATE_INSTANCE
         * @return Index of the newly created type.
         */
        uint32_t addVertexType(
            uint32_t const size,
            VkVertexInputRate const inputRate
        ) {
            uint32_t const index = vertexTypes.size();
            vertexTypes.push_back({index, size, inputRate});
            return index;
        }

        /**
         * @brief Add a vertex attribute description to a vertex type.
         * @param vertexTypeIndex Vertex type index (returned from addVertexType).
         * @param shaderLocation Location in the shader where this attribute should appear.
         * @param format Format of the attribute (e.g. VK_FORMAT_R32G32B32_SFLOAT).
         * @param offset Offset within the vertex datastructure in bytes.
         */
        void addVertexAttribute(
            uint32_t const vertexTypeIndex,
            uint32_t const shaderLocation,
            VkFormat const format,
            uint32_t const offset
        ) {
            if (vertexTypeIndex >= vertexTypes.size()) {
                throw std::runtime_error("Failed to add vertex attribute: Vertex type index out of range.");
            }

            attributes.push_back({vertexTypeIndex, shaderLocation, format, offset});
        }
    };


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

        VertexInfo vertexInfo;
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
