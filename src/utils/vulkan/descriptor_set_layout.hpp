#pragma once

#include "utils/misc/logging.hpp"
#include "utils/vulkan/handles.hpp"
#include "utils/vulkan/device.hpp"

#include "vulkan/vulkan.h"

#include <memory>


namespace utils::vulkan {

    /**
     * @brief Builder for initialisation of descriptor set layout objects.
     */
    class DescriptorSetLayoutBuilder {
    private:
        std::shared_ptr<Device> const device;

        std::vector<VkDescriptorSetLayoutBinding> descriptorBindings;

    public:
        /**
         * @brief Create a new descriptor set layout builder.
         * @param device Device
         */
        DescriptorSetLayoutBuilder(
            std::shared_ptr<Device> const& device
        ) : device(device) {}

        /**
         * @brief Add a descriptor to the descriptor set.
         * @param binding Index of the shader binding for the descriptor.
         * @param descriptorType Type of this descriptor binding.
         * @param stageFlags Stages of the shader pipeline to which the binding is required.
         * @param count Number of
         */
        DescriptorSetLayoutBuilder& addBinding(
            uint32_t const binding,
            VkDescriptorType const descriptorType,
            VkShaderStageFlags const stageFlags,
            uint32_t const count = 1
        ) {
            VkDescriptorSetLayoutBinding descriptorBinding {};
            descriptorBinding.binding = binding;
            descriptorBinding.descriptorType = descriptorType;
            descriptorBinding.descriptorCount = count;
            descriptorBinding.stageFlags = stageFlags;
            descriptorBinding.pImmutableSamplers = nullptr;

            descriptorBindings.push_back(descriptorBinding);

            return *this;
        }

    };


    class DescriptorSetLayout : public Handle<VkDescriptorSetLayout_T> {
    private:
        static utils::Logger log;

        std::shared_ptr<Device> const m_device;

    public:
        DescriptorSetLayout(
            std::shared_ptr<Device> const& device,
            DescriptorSetLayoutBuilder const& builder);
    };

}
