#include "utils/vulkan/swap_chain.hpp"


namespace utils::vulkan {

    utils::Logger SwapChain::log("SwapChain");


    void SwapChainPreferences::addSurfaceFormat(VkSurfaceFormatKHR const surfaceFormat) {
        surfaceFormats.push_back(surfaceFormat);
    }


    void SwapChainPreferences::addPresentMode(VkPresentModeKHR const presentMode) {
        presentModes.push_back(presentMode);
    }


    VkSurfaceFormatKHR SwapChainPreferences::selectSurfaceFormat(std::vector<VkSurfaceFormatKHR> const& deviceSupportedFormats) const {
        for (auto const& candidate : surfaceFormats) {
            for (auto const& supported : deviceSupportedFormats) {
                if (candidate.colorSpace == supported.colorSpace && candidate.format == supported.format) {
                    return candidate;
                }
            }
        }

        throw std::runtime_error("Could not find acceptable swap chain surface format.");
    }


    VkPresentModeKHR SwapChainPreferences::selectPresentMode(std::vector<VkPresentModeKHR> const& deviceSupportedModes) const {
        for (auto const& candidate : presentModes) {
            for (auto const& supported : deviceSupportedModes) {
                if (candidate == supported) {
                    return candidate;
                }
            }
        }

        throw std::runtime_error("Could not find acceptable swap chain present mode.");
    }


    SwapChain::SwapChain(
        std::shared_ptr<DeviceHandle> const& vkDeviceHandle,
        std::shared_ptr<SurfaceHandle> const& vkSurfaceHandle,
        SwapChainConfig const& config
    ) :
        vkSwapChainHandle(std::make_shared<SwapChainHandle>(vkDeviceHandle)),
        vkDeviceHandle(vkDeviceHandle),
        vkSurfaceHandle(vkSurfaceHandle),
        config(config)
    {
        INFO(log) << "Creating swap chain." << std::endl;

        VkSwapchainCreateInfoKHR createInfo {};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = vkSurfaceHandle->vk;
        createInfo.minImageCount = config.imageCount;
        createInfo.imageFormat = config.surfaceFormat.format;
        createInfo.imageColorSpace = config.surfaceFormat.colorSpace;
        createInfo.imageExtent = config.imageExtent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        auto const queueFamilyIndicesVector = std::vector<uint32_t>(config.queueFamilyIndices.begin(), config.queueFamilyIndices.end());

        if (config.queueFamilyIndices.size() > 1) {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = static_cast<uint32_t>(queueFamilyIndicesVector.size());
            createInfo.pQueueFamilyIndices = queueFamilyIndicesVector.data();
        } else {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0;
            createInfo.pQueueFamilyIndices = nullptr;
        }

        createInfo.preTransform = config.preTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = config.presentMode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = VK_NULL_HANDLE;

        if (vkCreateSwapchainKHR(this->vkDeviceHandle->vk, &createInfo, nullptr, &this->vkSwapChainHandle->vk)) {
            throw std::runtime_error("Failed to create swap chain.");
        }
    }

}
