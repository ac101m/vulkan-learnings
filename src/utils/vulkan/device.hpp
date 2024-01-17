#pragma once

#include "utils/vulkan/helpers.hpp"
#include "utils/vulkan/queue.hpp"
#include "utils/vulkan/swap_chain.hpp"
#include "utils/vulkan/shader_module.hpp"
#include "utils/vulkan/pipeline_layout.hpp"
#include "utils/vulkan/render_pass.hpp"
#include "utils/vulkan/graphics_pipeline.hpp"
#include "utils/vulkan/frame_buffer.hpp"
#include "utils/vulkan/command_pool.hpp"

#include "utils/misc/logging.hpp"

#include "vulkan/vulkan.h"

#include <vector>
#include <optional>
#include <memory>
#include <filesystem>


namespace utils::vulkan {

    class Device {
    private:
        static utils::Logger log;

        std::shared_ptr<InstanceHandle> vkInstanceHandle;
        std::shared_ptr<DeviceHandle> vkDeviceHandle;

        std::map<std::string, std::shared_ptr<Queue>> queueMap;

    private:
        void populateQueueMap(std::map<uint32_t, std::vector<std::string>> const& queueFamilyMap);

    public:
        /**
         * @brief Construct and initialize a new logical device instance.
         * @param vkInstanceHandle std::shared_ptr to utils::vulkan::Instance.
         * @param physicalDevice VkPhysicalDevice object instance designating the desired GPU.
         * @param queueFamilyIndexMap Map of queue names to queue family indices.
         * @param deviceExtensions Vector of strings specifying required device extensions.
         * @param validationLayerNames Vector of strings containing device validation layers (optional).
         */
        Device(
            std::shared_ptr<InstanceHandle> const& vkInstanceHandle,
            VkPhysicalDevice const& physicalDevice,
            std::map<std::string, uint32_t> const& queueFamilyIndexMap,
            std::vector<std::string> const& deviceExtensions,
            std::vector<std::string> const& validationLayerNames = std::vector<std::string>());

        /**
         * @brief Get a device queue.
         * @param name Name of the queue to retrieve (as passed in constructor map).
         * @return std::shared_ptr to a Queue object.
         */
        std::shared_ptr<Queue> getQueue(std::string const& queueName);

        /**
         * @brief Create a new swap chain.
         * @param surface Swap surface for the new swap chain.
         * @param swapChainConfig SwapChainConfig struct containing swap chain settings.
         */
        std::shared_ptr<SwapChain> createSwapChain(
            std::shared_ptr<Surface> const& surface,
            SwapChainConfig const& swapChainConfig) const;

        /**
         * @brief Create a new shader module from an spv file on disk.
         * @param path Path to the source SPIRV file to create the shader from.
         * @return Shared pointer to shader module object.
         */
        std::shared_ptr<ShaderModule> createShaderModule(std::filesystem::path const& path) const;

        /**
         * @brief Create a new pipeline layout with the provided configuration.
         * @param config Pipeline layout config object.
         * @return Shared pointer to new pipeline layout object.
         */
        std::shared_ptr<PipelineLayout> createPipelineLayout(PipelineLayoutConfig const& config) const;

        /**
         * @brief Create a new render pass.
         * @param config Render pass config object containing details of the render pass.
         * @return Shared pointer to new render pass object.
         */
        std::shared_ptr<RenderPass> createRenderPass(RenderPassConfig const& config) const;

        /**
         * @brief Create a new graphics pipeline.
         * @param config Graphics pipeline config object containing pipeline details.
         * @param pipelineLayout Shared pointer to valid pipeline layout object.
         * @param renderPass Shared pointer to valid render pass object.
         * @return Shared pointer to new graphics pipeline object.
         */
        std::shared_ptr<GraphicsPipeline> createGraphicsPipeline(
            std::shared_ptr<PipelineLayout> const& pipelineLayout,
            std::shared_ptr<RenderPass> const& renderPass,
            GraphicsPipelineConfig const& config) const;

        /**
         * @brief Create a new frame buffer from an image view.
         * @param imageView Shared pointer to image view to create frame buffer from.
         * @return Shared pointer to new frame buffer object.
         */
        std::shared_ptr<FrameBuffer> createFrameBuffer(
            std::shared_ptr<RenderPass> const& renderPass,
            FrameBufferConfig const& config) const;

        /**
         * @brief Create a new command buffer object.
         * @return Shared pointer to new command pool object.
         */
        std::shared_ptr<CommandPool> createCommandPool(CommandPoolConfig const& config) const;

        /**
         * @brief Return a reference to the device handle.
         * @return Shared pointer to device handle instance.
         */
        std::shared_ptr<DeviceHandle> getHandle() {
            return this->vkDeviceHandle;
        }

    };

}
