#include "utils/vulkan/image_view.hpp"


namespace utils::vulkan {

    utils::Logger ImageView::log("ImageView");


    ImageView::ImageView(
        std::shared_ptr<DeviceHandle> const& vkDeviceHandle,
        std::shared_ptr<ImageHandle> const& vkImageHandle,
        ImageViewConfig const& config
    ) :
        HandleWrapper<ImageViewHandle>(std::make_shared<ImageViewHandle>(vkDeviceHandle)),
        vkDeviceHandle(vkDeviceHandle),
        vkImageHandle(vkImageHandle),
        config(config)
    {
        INFO(log) << "Creating image view." << std::endl;

        VkImageViewCreateInfo createInfo {};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = this->vkImageHandle->vk;
        createInfo.viewType = config.viewType;
        createInfo.format = config.imageFormat;
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(this->vkDeviceHandle->vk, &createInfo, nullptr, &this->vkHandle->vk) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create image view.");
        }
    }

}
