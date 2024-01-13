#pragma once

#include "utils/misc/logging.hpp"
#include "utils/vulkan/handles.hpp"
#include "utils/vulkan/image.hpp"

#include <set>
#include <vector>


namespace utils::vulkan {

    /**
     * @brief Helper class to represent desired swap chain configuration.
     */
    class SwapChainPreferences {
    private:
        std::vector<VkSurfaceFormatKHR> surfaceFormats;
        std::vector<VkPresentModeKHR> presentModes;

    public:
        SwapChainPreferences() {}

        /**
         * @brief Add a surface format to the list of valid surface formats.
         */
        void addSurfaceFormat(VkSurfaceFormatKHR const surfaceFormat);

        /**
         * @brief Add a present mode to the list of valid present modes.
         */
        void addPresentMode(VkPresentModeKHR const presentMode);

        /**
         * @brief Select the first valid surface format from a vector of supported formats.
         * @param deviceSupportedFormats Vector of formats supported by the device.
         * @return The first valid format in the list of supported formats.
         */
        VkSurfaceFormatKHR selectSurfaceFormat(std::vector<VkSurfaceFormatKHR> const& deviceSupportedFormats) const;

        /**
         * @brief Select the first valid present mode from a vector of supported present modes.
         * @param deviceSupportedModes Vector of present modes supported by the device.
         * @return The first valid present mode in the list of supported present modes.
         */
        VkPresentModeKHR selectPresentMode(std::vector<VkPresentModeKHR> const& deviceSupportedModes) const;
    };


    /**
     * @brief Helper struct used in swap chain construction.
     */
    struct SwapChainConfig {
        VkSurfaceFormatKHR surfaceFormat;
        VkPresentModeKHR presentMode;
        uint32_t imageCount;
        VkExtent2D imageExtent;
        std::set<uint32_t> queueFamilyIndices;
        VkSurfaceTransformFlagBitsKHR preTransform;
    };


    class SwapChain {
    private:
        static utils::Logger log;

        std::shared_ptr<SwapChainHandle> const vkSwapChainHandle;
        std::shared_ptr<DeviceHandle> const vkDeviceHandle;
        std::shared_ptr<SurfaceHandle> const vkSurfaceHandle;

        std::vector<std::shared_ptr<Image>> images;

    public:
        SwapChainConfig const config;

    private:
        void populateImageList();

    public:
        SwapChain(
            std::shared_ptr<DeviceHandle> const& vkDeviceHandle,
            std::shared_ptr<SurfaceHandle> const& vkSurfaceHandle,
            SwapChainConfig const& config);

        /**
         * @brief Access swap chain images.
         * @param index uint32_t index of image to return.
         * @return std::shared_ptr<Image> of the selected image.
         */
        std::shared_ptr<Image> getImage(uint32_t const index);
    };

}
