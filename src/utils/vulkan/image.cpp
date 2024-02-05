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
        layout(config.initialLayout),
        mipLevelCount(config.mipLevelCount),
        layerCount(config.layerCount)
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

}
