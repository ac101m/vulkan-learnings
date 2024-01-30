#pragma once

#include "utils/misc/logging.hpp"
#include "utils/vulkan/handles.hpp"

#include "vulkan/vulkan.h"

#include <memory>


namespace utils::vulkan {

    /**
     * @brief Config object for initialisation of descriptor set layouts.
     */
    struct DescriptorSetLayoutConfig {
        std::vector<VkDescriptorSetLayoutBinding> descriptorBindings;

        /**
         * @brief Add a descriptor to the descriptor set config.
         * @param bindingIndex Index of the binding within the shader.
         * @param descriptorType Type of this descriptor binding.
         * @param stageFlags Stages of the shader pipeline to which the binding is required.
         */
        void addDescriptor(
            uint32_t const bindingIndex,
            VkDescriptorType const descriptorType,
            VkShaderStageFlags const stageFlags
        ) {
            VkDescriptorSetLayoutBinding descriptorBinding {};
            descriptorBinding.binding = bindingIndex;
            descriptorBinding.descriptorType = descriptorType;
            descriptorBinding.descriptorCount = 1;
            descriptorBinding.stageFlags = stageFlags;
            descriptorBinding.pImmutableSamplers = nullptr;

            descriptorBindings.push_back(descriptorBinding);
        }
    };


    class DescriptorSetLayout : public HandleWrapper<DescriptorSetLayoutHandle> {
    private:
        static utils::Logger log;

        std::shared_ptr<DeviceHandle> const vkDeviceHandle;

    public:
        DescriptorSetLayout(
            std::shared_ptr<DeviceHandle> const& vkDeviceHandle,
            DescriptorSetLayoutConfig const& config);
    };

}
