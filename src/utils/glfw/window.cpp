#include "utils/glfw/window.hpp"

#include <GLFW/glfw3.h>


namespace utils::glfw {

    Window::Window(std::string const name, uint32_t const width, uint32_t const height) : name(name) {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        INFO(logger) << "Creating window. "
                     << "name=" << name << ", "
                     << "width=" << width << ", "
                     << "height=" << height << std::endl;

        this->window = glfwCreateWindow(width, height, this->name.c_str(), nullptr, nullptr);
    }


    Window::~Window() {
        INFO(logger) << "Destroying window" << std::endl;
        glfwDestroyWindow(this->window);
    }


    bool Window::shouldClose() {
        return glfwWindowShouldClose(this->window);
    }

}
