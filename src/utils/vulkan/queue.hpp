#pragma once

#include "utils/misc/logging.hpp"
#include "utils/vulkan/helpers.hpp"
#include "utils/vulkan/semaphore.hpp"
#include "utils/vulkan/fence.hpp"
#include "utils/vulkan/command_buffer.hpp"
#include "utils/vulkan/swap_chain.hpp"

#include <memory>
#include <vector>


namespace utils::vulkan {

    struct waitCondition {
        VkPipelineStageFlags const waitStages;

    };


    class Queue {
    private:
        static utils::Logger log;

        std::shared_ptr<DeviceHandle> const vkDeviceHandle;

        VkQueue_T * vkQueue;

    public:
        std::string const name;
        uint32_t const queueFamilyIndex;

    public:
        Queue(
            std::shared_ptr<DeviceHandle> const& vkDeviceHandle,
            uint32_t const queueFamilyIndex,
            uint32_t const queueIndex,
            std::string const& name);

        /**
         * @brief Submit some work to the queue.
         * @param waitStages Vector of wait stage flags for each of the wait semaphores.
         * @param waitSemaphores Vector of semaphores that must be acquired before the buffer can execute.
         * @param signalSemaphores Vector of semaphores to signal once the operation is complete.
         * @param commandBuffers Vector of command buffers to schedule.
         * @param inFlightFence In-flight fence.
         */
        void submit(
            std::vector<VkPipelineStageFlags> const& waitStages,
            std::vector<std::shared_ptr<Semaphore>> const& waitSemaphores,
            std::vector<std::shared_ptr<Semaphore>> const& signalSemaphores,
            std::vector<std::shared_ptr<CommandBuffer>> const& commandBuffers,
            std::shared_ptr<Fence> const& inFlightFence);

        /**
         * @brief Return an image to the swap chain for presentation.
         * @param waitSemaphores Semaphores to wait on before executing the present operation.
         * @param swapChain The swap chain to present the image to.
         * @param imageIndex The index of the image in the swap chain to present.
         */
        void present(
            std::vector<std::shared_ptr<Semaphore>> const& waitSemaphores,
            std::shared_ptr<SwapChain> const& swapChain,
            uint32_t const imageIndex);
    };

}
