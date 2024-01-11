#pragma once

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>


namespace utils::glfw {

    /**
     * @brief Utility class for managing lifetime of GLFWwindow instances.
     */
    class WindowHandle {
    public:
        GLFWwindow * glfw;

        ~WindowHandle() {
            glfwDestroyWindow(this->glfw);
        }
    };

}
