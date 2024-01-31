#pragma once

#include "utils/vulkan/handles.hpp"
#include "utils/misc/logging.hpp"
#include "utils/vulkan/buffer.hpp"


namespace utils::vulkan {

    class DescriptorSet {
    private:
        static utils::Logger log;

        std::shared_ptr<DeviceHandle> const vkDeviceHandle;
        std::shared_ptr<DescriptorPoolHandle> const vkDescriptorPoolHandle;
        std::shared_ptr<DescriptorSetLayoutHandle> const vkDescriptorSetLayoutHandle;

    public:
        VkDescriptorSet_T * vk;

    public:
        DescriptorSet(
            std::shared_ptr<DeviceHandle> const& vkDeviceHandle,
            std::shared_ptr<DescriptorPoolHandle> const& vkDescriptorPoolHandle,
            std::shared_ptr<DescriptorSetLayoutHandle> const& vkDescriptorSetLayoutHandle);

        /**
         * @brief Update a descriptor set with data.
         * @param binding The index of the binding to update.
         * @param buffer A buffer to update the specified descriptor with.
         */
        void update(uint32_t const binding, std::shared_ptr<Buffer> const& buffer);
    };

}
