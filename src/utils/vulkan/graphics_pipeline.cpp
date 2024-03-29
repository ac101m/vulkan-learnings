#include "utils/vulkan/graphics_pipeline.hpp"


namespace utils::vulkan {

    utils::Logger GraphicsPipeline::log("GraphicsPipeline");


    GraphicsPipeline::GraphicsPipeline(
        std::shared_ptr<DeviceHandle> const& vkDeviceHandle,
        std::shared_ptr<PipelineLayoutHandle> const& vkPipelineLayoutHandle,
        std::shared_ptr<RenderPassHandle> const& vkRenderPassHandle,
        GraphicsPipelineConfig const& config
    ) :
        HandleWrapper<PipelineHandle>(std::make_shared<PipelineHandle>(vkDeviceHandle)),
        vkDeviceHandle(vkDeviceHandle),
        vkPipelineLayoutHandle(vkPipelineLayoutHandle),
        vkRenderPassHandle(vkRenderPassHandle),
        config(config)
    {
        INFO(log) << "Creating new graphics pipeline." << std::endl;

        VkPipelineShaderStageCreateInfo shaderStages[config.shaderStages.size()];
        for (unsigned i = 0; i < config.shaderStages.size(); i++) {
            shaderStages[i] = config.shaderStages[i].toVulkan();
        }

        VkPipelineVertexInputStateCreateInfo vertexInputInfo {};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(config.vertexInfo.vertexTypes.size());
        vertexInputInfo.pVertexBindingDescriptions = config.vertexInfo.vertexTypes.data();
        vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(config.vertexInfo.attributes.size());
        vertexInputInfo.pVertexAttributeDescriptions = config.vertexInfo.attributes.data();

        // TODO - Add to config
        VkPipelineInputAssemblyStateCreateInfo inputAssembly {};
        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssembly.primitiveRestartEnable = VK_FALSE;

        // TODO - Add to config
/*
        VkViewport viewport {};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float) this->vkSwapChain->config.imageExtent.width;
        viewport.height = (float) this->vkSwapChain->config.imageExtent.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor {};
        scissor.offset = {0, 0};
        scissor.extent = this->vkSwapChain->config.imageExtent;
*/
        // TODO - Add to config
        std::vector<VkDynamicState> const dynamicStates = {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR
        };

        // TODO - Add to config
        VkPipelineDynamicStateCreateInfo dynamicState {};
        dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
        dynamicState.pDynamicStates = dynamicStates.data();

        // TODO - Add to config
        VkPipelineViewportStateCreateInfo viewportState {};
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.scissorCount = 1;

        // TODO - Add to config
        VkPipelineRasterizationStateCreateInfo rasterizer {};
        rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.depthClampEnable = VK_FALSE;
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizer.lineWidth = 1.0f;
        rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
        rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
        rasterizer.depthBiasEnable = VK_FALSE;
        rasterizer.depthBiasConstantFactor = 0.0f;          // Optional
        rasterizer.depthBiasClamp = 0.0f;                   // Optional
        rasterizer.depthBiasSlopeFactor = 0.0f;             // Optional

        // TODO - Add to config
        VkPipelineMultisampleStateCreateInfo multisampling {};
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.sampleShadingEnable = VK_FALSE;
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        multisampling.minSampleShading = 1.0f;              // Optional
        multisampling.pSampleMask = nullptr;                // Optional
        multisampling.alphaToCoverageEnable = VK_FALSE;     // Optional
        multisampling.alphaToOneEnable = VK_FALSE;          // Optional

        // TODO - Add to config
        VkPipelineColorBlendAttachmentState colorBlendAttachment {};
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_FALSE;
        colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;     // Optional
        colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;    // Optional
        colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;                // Optional
        colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;     // Optional
        colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;    // Optional
        colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;                // Optional

        // TODO - Add to config
        VkPipelineColorBlendStateCreateInfo colorBlending{};
        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.logicOp = VK_LOGIC_OP_COPY;           // Optional
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;
        colorBlending.blendConstants[0] = 0.0f;             // Optional
        colorBlending.blendConstants[1] = 0.0f;             // Optional
        colorBlending.blendConstants[2] = 0.0f;             // Optional
        colorBlending.blendConstants[3] = 0.0f;             // Optional

        VkGraphicsPipelineCreateInfo pipelineInfo {};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = static_cast<uint32_t>(config.shaderStages.size());
        pipelineInfo.pStages = shaderStages;
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pMultisampleState = &multisampling;
        pipelineInfo.pDepthStencilState = nullptr;              // Optional
        pipelineInfo.pColorBlendState = &colorBlending;
        pipelineInfo.pDynamicState = &dynamicState;
        pipelineInfo.layout = this->vkPipelineLayoutHandle->vk;
        pipelineInfo.renderPass = this->vkRenderPassHandle->vk;
        pipelineInfo.subpass = 0;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;       // Optional
        pipelineInfo.basePipelineIndex = -1;                    // Optional

        if (vkCreateGraphicsPipelines(this->vkDeviceHandle->vk, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &this->vkHandle->vk) != VK_SUCCESS) {
            throw std::runtime_error("failed to create graphics pipeline.");
        }
    }

}
