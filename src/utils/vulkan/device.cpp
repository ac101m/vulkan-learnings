#include "utils/vulkan/device.hpp"

#include "vulkan/vulkan.h"


namespace utils::vulkan {

    utils::Logger Device::log = Logger("Device");


    std::map<uint32_t, std::vector<std::string>> getQueueFamilyMap(std::map<std::string, uint32_t> const& queueFamilyIndexMap) {
        std::map<uint32_t, std::vector<std::string>> queueFamilyMap;

        for (auto const& entry : queueFamilyIndexMap) {
            auto const& queueName = entry.first;
            auto const& queueFamilyIndex = entry.second;

            if (queueFamilyMap.find(queueFamilyIndex) == queueFamilyMap.end()) {
                queueFamilyMap[queueFamilyIndex] = std::vector<std::string>();
            }

            queueFamilyMap[queueFamilyIndex].push_back(queueName);
        }

        return queueFamilyMap;
    }


    Device::Device(
        std::shared_ptr<InstanceHandle> const& vkInstanceHandle,
        VkPhysicalDevice const& physicalDevice,
        std::map<std::string, uint32_t> const& queueFamilyIndexMap,
        std::vector<std::string> const& deviceExtensions,
        std::vector<std::string> const& validationLayerNames
    ) :
        vkInstanceHandle(vkInstanceHandle),
        vkDeviceHandle(std::make_shared<DeviceHandle>())
    {
        INFO(log) << "Creating logical device." << std::endl;

        auto const queueFamilyMap = getQueueFamilyMap(queueFamilyIndexMap);

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        float queuePriority = 1.0f;

        for (auto const& entry : queueFamilyMap) {
            uint32_t queueFamilyIndex = entry.first;
            uint32_t queueFamilyFrequency = entry.second.size();

            VkDeviceQueueCreateInfo queueCreateInfo {};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
            queueCreateInfo.queueCount = queueFamilyFrequency;
            queueCreateInfo.pQueuePriorities = &queuePriority;

            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkPhysicalDeviceFeatures deviceFeatures {};

        auto const validationLayerParams = StringParameters(validationLayerNames);
        auto const deviceExtensionParams = StringParameters(deviceExtensions);

        VkDeviceCreateInfo createInfo {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.pEnabledFeatures = &deviceFeatures;
        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayerParams.size());
        createInfo.ppEnabledLayerNames = validationLayerParams.data();
        createInfo.enabledExtensionCount = deviceExtensionParams.size();
        createInfo.ppEnabledExtensionNames = deviceExtensionParams.data();

        if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &this->vkDeviceHandle->vk) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create logical device.");
        }

        populateQueueMap(queueFamilyMap);
    }


    void Device::populateQueueMap(std::map<uint32_t, std::vector<std::string>> const& queueFamilyMap) {
        for (auto const& entry : queueFamilyMap) {
            auto const& queueFamilyIndex = entry.first;
            auto const& queueNames = entry.second;

            for (unsigned i = 0; i < queueNames.size(); i++) {
                auto const& queueName = queueNames[i];
                auto const queue = std::make_shared<Queue>(this->vkDeviceHandle, queueFamilyIndex, i, queueNames[i]);
                this->queueMap.insert(std::make_pair(queueName, queue));
            }
        }
    }


    std::shared_ptr<Queue> Device::getQueue(std::string const& queueName) {
        if (this->queueMap.find(queueName) == this->queueMap.end()) {
            throw std::runtime_error("No queue with specified name.");
        }

        return this->queueMap[queueName];
    }


    std::shared_ptr<SwapChain> Device::createSwapChain(
        std::shared_ptr<Surface> const& surface,
        SwapChainConfig const& swapChainConfig
    ) const {
        return std::make_shared<SwapChain>(this->vkDeviceHandle, surface->getHandle(), swapChainConfig);
    }


    std::shared_ptr<ShaderModule> Device::createShaderModule(std::filesystem::path const& path) const {
        return std::make_shared<ShaderModule>(this->vkDeviceHandle, path);
    }


    std::shared_ptr<PipelineLayout> Device::createPipelineLayout(PipelineLayoutConfig const& config) const {
        return std::make_shared<PipelineLayout>(this->vkDeviceHandle, config);
    }


    std::shared_ptr<RenderPass> Device::createRenderPass(RenderPassConfig const& config) const {
        return std::make_shared<RenderPass>(this->vkDeviceHandle, config);
    }


    std::shared_ptr<GraphicsPipeline> Device::createGraphicsPipeline(
        std::shared_ptr<PipelineLayout> const& pipelineLayout,
        std::shared_ptr<RenderPass> const& renderPass,
        GraphicsPipelineConfig const& config
    ) const {
        return std::make_shared<GraphicsPipeline>(
            this->vkDeviceHandle, pipelineLayout->getHandle(), renderPass->getHandle(), config);
    }
}
