#include "utils/glfw/window.hpp"

#include <GLFW/glfw3.h>


namespace utils::glfw {

    utils::Logger Window::log = utils::Logger("Window");


    Window::Window(std::string const name, uint32_t const width, uint32_t const height) :
        windowHandle(std::make_shared<WindowHandle>()), name(name)
    {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        INFO(log) << "Creating window. "
                  << "name=" << name << ", "
                  << "width=" << width << ", "
                  << "height=" << height << std::endl;

        this->windowHandle->glfw = glfwCreateWindow(width, height, this->name.c_str(), nullptr, nullptr);
    }


    VkExtent2D Window::getFrameBufferSize() const {
        int width;
        int height;

        glfwGetFramebufferSize(this->windowHandle->glfw, &width, &height);

        return {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)
        };
    }


    bool Window::shouldClose() {
        return glfwWindowShouldClose(this->windowHandle->glfw);
    }

}
