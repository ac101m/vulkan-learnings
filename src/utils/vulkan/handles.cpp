#include "utils/vulkan/handles.hpp"
#include "utils/vulkan/helpers.hpp"


namespace utils::vulkan {

    DebugMessengerHandle::~DebugMessengerHandle() {
        DestroyDebugUtilsMessengerEXT(this->instanceHandle->vk, this->vk, nullptr);
    }

}
