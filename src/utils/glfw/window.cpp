#include "utils/glfw/window.hpp"

#include <GLFW/glfw3.h>


namespace utils::glfw {

    Window::Window(uint32_t const width, uint32_t const height) {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        this->window = glfwCreateWindow(width, height, "Vulkan", nullptr, nullptr);
    }


    Window::~Window() {
        glfwDestroyWindow(this->window);
    }


    bool Window::shouldClose() {
        return glfwWindowShouldClose(this->window);
    }

}
