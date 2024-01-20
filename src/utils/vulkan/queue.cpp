#include "utils/vulkan/queue.hpp"


namespace utils::vulkan {

    utils::Logger Queue::log("Queue");


    Queue::Queue(
        std::shared_ptr<DeviceHandle> const& vkDeviceHandle,
        uint32_t const queueFamilyIndex,
        uint32_t const queueIndex,
        std::string const& name
    ) : vkDeviceHandle(vkDeviceHandle), name(name), queueFamilyIndex(queueFamilyIndex) {
        INFO(log) << "Creating queue '" << name << "' family index " << queueFamilyIndex << std::endl;
        vkGetDeviceQueue(vkDeviceHandle->vk, queueFamilyIndex, queueIndex, &vkQueue);
    }


    void Queue::submit(
        std::vector<VkPipelineStageFlags> const& waitStages,
        std::vector<std::shared_ptr<Semaphore>> const& waitSemaphores,
        std::vector<std::shared_ptr<Semaphore>> const& signalSemaphores,
        std::vector<std::shared_ptr<CommandBuffer>> const& commandBuffers,
        std::shared_ptr<Fence> const& inFlightFence
    ) {
        if (waitStages.size() != waitSemaphores.size()) {
            throw std::runtime_error("Unable to submit work to queue, wait stage count does not match wait semaphore count.");
        }

        VkSemaphore waitSemaphoreArray[waitSemaphores.size()];
        for (unsigned i = 0; i < waitSemaphores.size(); i++) {
            waitSemaphoreArray[i] = waitSemaphores[i]->getHandle()->vk;
        }

        VkSemaphore signalSemaphoreArray[signalSemaphores.size()];
        for (unsigned i = 0; i < signalSemaphores.size(); i++) {
            signalSemaphoreArray[i] = signalSemaphores[i]->getHandle()->vk;
        }

        VkCommandBuffer commandBuffersArray[commandBuffers.size()];
        for (unsigned i = 0; i < commandBuffers.size(); i++) {
            commandBuffersArray[i] = commandBuffers[i]->vk;
        }

        VkSubmitInfo submitInfo {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.waitSemaphoreCount = static_cast<uint32_t>(waitSemaphores.size());
        submitInfo.pWaitSemaphores = waitSemaphoreArray;
        submitInfo.pWaitDstStageMask = waitStages.data();
        submitInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());
        submitInfo.pCommandBuffers = commandBuffersArray;
        submitInfo.signalSemaphoreCount = static_cast<uint32_t>(signalSemaphores.size());
        submitInfo.pSignalSemaphores = signalSemaphoreArray;

        if (vkQueueSubmit(this->vkQueue, 1, &submitInfo, inFlightFence->getHandle()->vk) != VK_SUCCESS) {
            throw std::runtime_error("failed to submit draw command buffer.");
        }
    }


    void Queue::present(
        std::vector<std::shared_ptr<Semaphore>> const& waitSemaphores,
        std::shared_ptr<SwapChain> const& swapChain,
        uint32_t const imageIndex
    ) {
        VkSemaphore waitSemaphoreArray[waitSemaphores.size()];
        for (unsigned i = 0; i < waitSemaphores.size(); i++) {
            waitSemaphoreArray[i] = waitSemaphores[i]->getHandle()->vk;
        }

        VkPresentInfoKHR presentInfo {};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.pWaitSemaphores = waitSemaphoreArray;
        presentInfo.waitSemaphoreCount = static_cast<uint32_t>(waitSemaphores.size());
        presentInfo.pSwapchains = &swapChain->getHandle()->vk;
        presentInfo.swapchainCount = 1;
        presentInfo.pImageIndices = &imageIndex;
        presentInfo.pResults = nullptr; // optional

        vkQueuePresentKHR(this->vkQueue, &presentInfo);
    }
}
