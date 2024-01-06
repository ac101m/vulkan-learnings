#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdint.h>


namespace utils::glfw {

    class Window {
    private:
        GLFWwindow* window = nullptr;

    public:
        Window(uint32_t const width, uint32_t const height);
        ~Window();

        /**
         * @brief Wrapper around glfwWindowShouldClose(window).
         * @return True if window should close, false otherwise.
         */
        bool shouldClose();
    };

}
