#include "utils/vulkan/surface.hpp"


namespace utils::vulkan {

    utils::Logger Surface::log("Surface");


    Surface::Surface(std::shared_ptr<InstanceHandle> const& instanceHandle, std::shared_ptr<utils::glfw::WindowHandle> const& windowHandle) :
        surfaceHandle(std::make_shared<SurfaceHandle>(instanceHandle)), instanceHandle(instanceHandle), windowHandle(windowHandle)
    {
        INFO(log) << "Creating surface." << std::endl;

        if (glfwCreateWindowSurface(this->instanceHandle->vk, this->windowHandle->glfw, nullptr, &this->surfaceHandle->vk) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create window surface.");
        }
    }

}
