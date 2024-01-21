#include "utils/vulkan/image.hpp"


namespace utils::vulkan {

    utils::Logger Image::log("Image");


    Image::Image(std::shared_ptr<ImageHandle> const& vkImageHandle, std::shared_ptr<DeviceHandle> const& vkDeviceHandle) :
        HandleWrapper<ImageHandle>(vkImageHandle), vkDeviceHandle(vkDeviceHandle)
    {
        INFO(log) << "Wrapping swap chain image." << std::endl;
    }


    std::shared_ptr<ImageView> Image::createImageView(ImageViewConfig const& config) {
        return std::make_shared<ImageView>(this->vkDeviceHandle, this->vkHandle, config);
    }
}
