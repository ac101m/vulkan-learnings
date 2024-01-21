#pragma once

#include "utils/misc/logging.hpp"
#include "utils/vulkan/handles.hpp"
#include "utils/vulkan/image.hpp"
#include "utils/vulkan/semaphore.hpp"

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


    class SwapChain : public HandleWrapper<SwapChainHandle> {
    private:
        static utils::Logger log;

        std::shared_ptr<DeviceHandle> const vkDeviceHandle;
        std::shared_ptr<SurfaceHandle> const vkSurfaceHandle;

        std::vector<std::shared_ptr<Image>> images;
        std::vector<std::shared_ptr<ImageView>> imageViews;

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
         * @return Vector containing shared pointers to swap chain images.
         */
        std::vector<std::shared_ptr<Image>> getImages() const;

        /**
         * @brief Create image views for all swap chain images.
         * @param config ImageViewConfig containing image view configuration info.
         * @return Vector containing shared pointers to image views.
         */
        std::vector<std::shared_ptr<ImageView>> createImageViews(ImageViewConfig const& config);

        /**
         * @brief Get the index of the next image.
         * @param imageAvailableSemaphore A semaphore to signal once the image is available.
         * @param result Pointer to VkResult instance to check status of call (optional).
         * @return Index of the next image in the swap chain.
         */
        uint32_t getNextImage(std::shared_ptr<Semaphore> const& imageAvailableSemaphore, VkResult * const resultOut = nullptr);
    };

}
