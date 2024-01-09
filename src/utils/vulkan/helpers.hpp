#pragma once

#include "utils/misc/string.hpp"

#include "vulkan/vulkan.h"

#include <vector>
#include <string>


namespace utils::vulkan {

    /**
     * @brief Get vector of vulkan extensions supported by the current platform.
     * @return vector of VkExtensionProperties objects.
     */
    std::vector<VkExtensionProperties> getSupportedExtensions();


    /**
     * @brief Utility class to manage the passing of parameters to vulkan API functions.
     */
    class StringParameters {
    private:
        std::vector<const char *> pointers;

    public:
        StringParameters(std::vector<std::string> const& parameters) {
            pointers.reserve(parameters.size());
            for (auto const& parameter : parameters) {
                pointers.push_back(utils::createRawString(parameter));
            }
        }

        ~StringParameters() {
            for (auto const& pointer : pointers) {
                delete[] pointer;
            }
        }

        uint32_t size() const {
            return static_cast<uint32_t>(pointers.size());
        }

        char const * const * data() const {
            return pointers.data();
        }
    };


    /**
     * @brief Base class for vulkan object handles.
     */
    template<typename T>
    struct VkHandle {
        T * vk;
    };


    /**
     * @brief Class for managing lifetime of VkInstance objects.
     */
    struct InstanceHandle : VkHandle<VkInstance_T> {
        ~InstanceHandle() {
            vkDestroyInstance(this->vk, nullptr);
        }
    };


    /**
     * @brief Class for managing lifetime of VkDevice objects;
     */
    struct DeviceHandle : VkHandle<VkDevice_T> {
        ~DeviceHandle() {
            vkDestroyDevice(this->vk, nullptr);
        }
    };
}
