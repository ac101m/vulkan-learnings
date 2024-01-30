#pragma once

#include "utils/misc/logging.hpp"
#include "utils/vulkan/handles.hpp"
#include "utils/vulkan/helpers.hpp"
#include "utils/vulkan/descriptor_set_layout.hpp"

#include <memory>


namespace utils::vulkan {

    struct PipelineLayoutConfig {
        std::vector<std::shared_ptr<DescriptorSetLayout>> descriptorSets;

        /**
         * @brief Add a descriptor set to the configuration.
         * @param descriptorSet The descriptor set to add to the configuration.
         */
        void addDescriptorSet(std::shared_ptr<DescriptorSetLayout> const& descriptorSet) {
            descriptorSets.push_back(descriptorSet);
        }
    };


    class PipelineLayout : public HandleWrapper<PipelineLayoutHandle> {
    private:
        static utils::Logger log;

        std::shared_ptr<DeviceHandle> const vkDeviceHandle;

        PipelineLayoutConfig const config;

    public:
        PipelineLayout(
            std::shared_ptr<DeviceHandle> const& vkDeviceHandle,
            PipelineLayoutConfig const& config);
    };

}
