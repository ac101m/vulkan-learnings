#pragma once

#include "utils/misc/string.hpp"
#include "utils/vulkan/surface.hpp"

#include "vulkan/vulkan.h"

#include <vector>
#include <string>
#include <map>
#include <memory>


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
     * @brief Helper class for representing the requirements of a queue.
     */
    struct QueueConstraints {
        uint32_t requiredFlags;
        std::shared_ptr<Surface> presentSurface;

        /**
         * @brief All parameters constructor.
         * @param requiredFlags Required queue flags.
         * @param presentSurface Shared pointer to a present surface that the queue must support.
         */
        QueueConstraints(
            uint32_t const requiredFlags,
            std::shared_ptr<Surface> const& presentSurface
        ):
            requiredFlags(requiredFlags),
            presentSurface(presentSurface)
        {}
    };


    /**
     * @brief Helper class for containing named queue requirements.
     */
    class QueuePlan {
    public:
        std::map<std::string, QueueConstraints> queues;

        void addQueue(std::string const& name, QueueConstraints const& requirements) {
            queues.insert(std::make_pair(name, requirements));
        }
    };

}
