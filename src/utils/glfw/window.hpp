#pragma once

#include "utils/misc/logging.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdint.h>


namespace utils::glfw {

    /**
     * @brief RAII wrapper for GLFW window object.
     */
    class Window {
    private:
        GLFWwindow* window = nullptr;

        std::string const name;

        static utils::Logger log;

    public:
        Window(std::string const name, uint32_t const width, uint32_t const height);
        ~Window();

        /**
         * @brief Wrapper around glfwWindowShouldClose(window).
         * @return True if window should close, false otherwise.
         */
        bool shouldClose();
    };

}
