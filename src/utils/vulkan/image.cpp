#include "utils/vulkan/image.hpp"


namespace utils::vulkan {

    utils::Logger Image::log("Image");


    Image::Image(std::shared_ptr<ImageHandle> const& vkImageHandle, std::shared_ptr<DeviceHandle> const& vkDeviceHandle) :
        HandleWrapper<ImageHandle>(vkImageHandle), vkDeviceHandle(vkDeviceHandle)
    {
        INFO(log) << "Wrapping swap chain image." << std::endl;
    }


    Image::Image(
        std::shared_ptr<DeviceHandle> const& vkDeviceHandle,
        ImageConfig const& config
    ) :
        HandleWrapper<ImageHandle>(std::make_shared<ImageHandle>(vkDeviceHandle)),
        vkDeviceHandle(vkDeviceHandle),
        mutableSettings(config.getMutableSettings())
    {
        INFO(log) << "Creating image." << std::endl;

        VkImageCreateInfo imageInfo {};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = config.type;
        imageInfo.extent.width = config.width;
        imageInfo.extent.height = config.height;
        imageInfo.extent.depth = config.depth;
        imageInfo.mipLevels = config.mipLevelCount;
        imageInfo.arrayLayers = config.layerCount;
        imageInfo.format = config.format;
        imageInfo.tiling = config.tiling;
        imageInfo.initialLayout = config.initialLayout;
        imageInfo.usage = config.usage;
        imageInfo.sharingMode = config.sharingMode;
        imageInfo.samples = config.samples;

        if (vkCreateImage(this->vkDeviceHandle->vk, &imageInfo, nullptr, &this->vkHandle->vk) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create image.");
        }
    }


    VkMemoryRequirements Image::getMemoryRequirements() {
        VkMemoryRequirements requirements {};
        vkGetImageMemoryRequirements(this->vkDeviceHandle->vk, this->vkHandle->vk, &requirements);
        return requirements;
    }


    void Image::bindMemory(std::shared_ptr<DeviceMemory> const& deviceMemory, uint64_t const offset) {
        vkBindImageMemory(this->vkDeviceHandle->vk, this->vkHandle->vk, deviceMemory->getHandle()->vk, offset);
    }


    std::shared_ptr<ImageView> Image::createImageView(ImageViewConfig const& config) {
        return std::make_shared<ImageView>(this->vkDeviceHandle, this->vkHandle, config);
    }


    VkImageMemoryBarrier Image::updateSettings(MutableImageSettings const& newSettings) {
        auto const& oldSettings = this->mutableSettings;

        VkImageMemoryBarrier barrier {};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.image = this->vkHandle->vk;
        barrier.oldLayout = oldSettings.layout;
        barrier.newLayout = newSettings.layout;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = newSettings.mipLevelCount;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = newSettings.layerCount;
        barrier.srcAccessMask = 0;  // TODO
        barrier.dstAccessMask = 0;  // TODO

        this->mutableSettings = newSettings;

        return barrier;
    }

}
