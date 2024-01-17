#pragma once

#include "utils/misc/logging.hpp"
#include "utils/vulkan/handles.hpp"
#include "utils/glfw/handles.hpp"

#include <memory>


namespace utils::vulkan {

    class Surface {
    private:
        static utils::Logger log;

        std::shared_ptr<SurfaceHandle> const surfaceHandle;
        std::shared_ptr<InstanceHandle> const instanceHandle;
        std::shared_ptr<utils::glfw::WindowHandle> const windowHandle;

    public:
        Surface(std::shared_ptr<InstanceHandle> const& instanceHandle, std::shared_ptr<utils::glfw::WindowHandle> const& windowHandle);

        VkExtent2D getSurfaceExtent() const;

        /**
         * @brief Return a reference to the surface handle.
         * @return Shared pointer to surface handle instance.
         */
        std::shared_ptr<SurfaceHandle> getHandle() {
            return this->surfaceHandle;
        }
    };

}
