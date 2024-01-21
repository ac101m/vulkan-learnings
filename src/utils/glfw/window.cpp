#include "utils/glfw/window.hpp"

#include <GLFW/glfw3.h>


namespace utils::glfw {

    utils::Logger Window::log = utils::Logger("Window");


    void Window::resizeCallback(GLFWwindow * window, int width, int height) {
        INFO(log) << "Window resized width=" << width << ", height=" << height << std::endl;
        auto windowPtr = reinterpret_cast<Window *>(glfwGetWindowUserPointer(window));
        windowPtr->resized = true;
    }


    Window::Window(std::string const name, uint32_t const width, uint32_t const height, bool const resizeable) :
        windowHandle(std::make_shared<WindowHandle>()), name(name)
    {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, resizeable ? GLFW_TRUE : GLFW_FALSE);

        INFO(log) << "Creating window. "
                  << "name=" << name << ", "
                  << "width=" << width << ", "
                  << "height=" << height << ", "
                  << "resizeable=" << resizeable << std::endl;

        this->windowHandle->glfw = glfwCreateWindow(width, height, this->name.c_str(), nullptr, nullptr);

        glfwSetWindowUserPointer(this->windowHandle->glfw, this);
        glfwSetFramebufferSizeCallback(this->windowHandle->glfw, resizeCallback);
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
