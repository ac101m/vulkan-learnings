#pragma once

#include "utils/vulkan/handles.hpp"
#include "utils/misc/logging.hpp"
#include "utils/vulkan/descriptor_set.hpp"
#include "utils/vulkan/descriptor_set_layout.hpp"

#include <memory>


namespace utils::vulkan {

    /**
     * @brief Configuration struct for creating descriptor pools.
     */
    struct DescriptorPoolConfig {
        uint32_t const maxSets;
        std::vector<VkDescriptorPoolSize> poolSizes;

        /**
         * @brief Add a pool to the configuration.
         * @param poolType Type for this descriptor pool.
         * @param poolSize Size of this descriptor pool.
         */
        void addPool(VkDescriptorType poolType, uint32_t const poolSize) {
            VkDescriptorPoolSize poolSizeInfo {};
            poolSizeInfo.type = poolType;
            poolSizeInfo.descriptorCount = poolSize;
            poolSizes.push_back(poolSizeInfo);
        }

        DescriptorPoolConfig(uint32_t const maxSets)
            : maxSets(maxSets) {}
    };


    class DescriptorPool: public HandleWrapper<DescriptorPoolHandle> {
    private:
        static utils::Logger log;

        std::shared_ptr<DeviceHandle> const vkDeviceHandle;

    public:
        DescriptorPool(
            std::shared_ptr<DeviceHandle> const& vkDeviceHandle,
            DescriptorPoolConfig const& config);

        /**
         * @brief Allocate a descriptor set.
         * @param layout Layout of the descriptor set.
         * @return Shared pointer to newly allocated descriptor set.
         */
        std::shared_ptr<DescriptorSet> allocateDescriptorSet(std::shared_ptr<DescriptorSetLayout> const& layout);
    };

}
