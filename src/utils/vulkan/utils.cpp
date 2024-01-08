#include "utils/vulkan/utils.hpp"


namespace utils::vulkan {

    std::vector<VkExtensionProperties> getSupportedExtensions() {
        uint32_t vkExtensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &vkExtensionCount, nullptr);

        std::vector<VkExtensionProperties> vkExtensions(vkExtensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &vkExtensionCount, vkExtensions.data());

        return vkExtensions;
    }

}
