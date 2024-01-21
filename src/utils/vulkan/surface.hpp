#pragma once

#include "utils/misc/logging.hpp"
#include "utils/vulkan/handles.hpp"
#include "utils/glfw/handles.hpp"

#include <memory>


namespace utils::vulkan {

    class Surface : public HandleWrapper<SurfaceHandle> {
    private:
        static utils::Logger log;

        std::shared_ptr<InstanceHandle> const instanceHandle;
        std::shared_ptr<utils::glfw::WindowHandle> const windowHandle;

    public:
        Surface(std::shared_ptr<InstanceHandle> const& instanceHandle, std::shared_ptr<utils::glfw::WindowHandle> const& windowHandle);

        /**
         * @brief Get the size of a present surface.
         * @return VkExtent2D containing surface stize.
         */
        VkExtent2D getSurfaceExtent() const;
    };

}
