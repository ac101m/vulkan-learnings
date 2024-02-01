#pragma once

#include "utils/vulkan/handles.hpp"
#include "utils/misc/logging.hpp"
#include "utils/vulkan/descriptor_set.hpp"
#include "utils/vulkan/descriptor_set_layout.hpp"
#include "utils/vulkan/device.hpp"

#include <memory>
#include <vector>


namespace utils::vulkan {

    /**
     * @brief Builder for creating descriptor pools.
     */
    class DescriptorPoolBuilder {
    private:
        uint32_t const m_maxSets;
        std::vector<VkDescriptorPoolSize> poolSizes;

    public:
        DescriptorPoolBuilder(
            std::shared_ptr<Device> const& device,
            uint32_t const maxSets
        ) : m_maxSets(maxSets) {}

        /**
         * @brief Add a pool to the configuration.
         * @param poolType Type for this descriptor pool.
         * @param poolSize Size of this descriptor pool.
         */
        DescriptorPoolBuilder& addPool(VkDescriptorType poolType, uint32_t const poolSize) {
            VkDescriptorPoolSize poolSizeInfo {};
            poolSizeInfo.type = poolType;
            poolSizeInfo.descriptorCount = poolSize;

            poolSizes.push_back(poolSizeInfo);

            return *this;
        }
    };


    class DescriptorPool: public HandleWrapper<DescriptorPoolHandle> {
    private:
        static utils::Logger log;

        std::shared_ptr<DeviceHandle> const vkDeviceHandle;

    public:
        DescriptorPool(DescriptorPoolBuilder const& DescriptorPoolBuilder);

        /**
         * @brief Allocate a descriptor set.
         * @param layout Layout of the descriptor set.
         * @return Shared pointer to newly allocated descriptor set.
         */
        std::shared_ptr<DescriptorSet> allocateDescriptorSet(std::shared_ptr<DescriptorSetLayout> const& layout);
    };

}
