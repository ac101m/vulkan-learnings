#include "utils/vulkan/image.hpp"


namespace utils::vulkan {

    utils::Logger Image::log("Image");


    Image::Image(VkImage const& vkImage, std::shared_ptr<SwapChainHandle> const& vkSwapChainHandle) :
        vkImage(vkImage), vkSwapChainHandle(vkSwapChainHandle)
    {
        INFO(log) << "Creating image." << std::endl;
    }

}
