#pragma once

#define GLFW_INCLUDE_VULKAN

#include "utils/misc/logging.hpp"
#include "utils/glfw/handles.hpp"

#include <GLFW/glfw3.h>

#include <stdint.h>
#include <memory>
#include <string>


namespace utils::glfw {

    /**
     * @brief RAII wrapper for GLFW window object.
     */
    class Window {
    private:
        static utils::Logger log;

        std::shared_ptr<WindowHandle> windowHandle;

        std::string const name;

    public:
        Window(std::string const name, uint32_t const width, uint32_t const height);

        /**
         * @brief Return a reference counted copy of the window handle.
         * @return Shared pointer of glfw window handle.
         */
        std::shared_ptr<WindowHandle> getHandle() {
            return this->windowHandle;
        }

        /**
         * @brief Wrapper around glfwWindowShouldClose(window).
         * @return True if window should close, false otherwise.
         */
        bool shouldClose();
    };

}
