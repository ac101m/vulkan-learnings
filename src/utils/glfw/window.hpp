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

        bool resized = false;

    private:
        static void resizeCallback(GLFWwindow * window, int width, int height);

    public:
        /**
         * @brief Open a new glfw window.
         * @param name Name to give the window.
         * @param width Width of the window in pixels.
         * @param height Height of the window in pixels.
         * @param resizeable true to allow window resizing, false otherwise.
         */
        Window(std::string const name, uint32_t const width, uint32_t const height, bool const resizeable);

        /**
         * @brief Wrapper around glfwWindowShouldClose(window).
         * @return True if window should close, false otherwise.
         */
        bool shouldClose();

        /**
         * @brief Get the size of the frame buffer.
         * @return VkExtent2D containing frame buffer size in pixels.
         */
        VkExtent2D getFrameBufferSize() const;

        /**
         * @brief Get the resized state.
         * @return boolean containing resize state.
         */
        bool hasResized() const {
            return resized;
        }

        /**
         * @brief Reset the resized state.
         */
        void resetResized() {
            resized = false;
        }

        /**
         * @brief Return a reference counted copy of the window handle.
         * @return Shared pointer to glfw window handle instance.
         */
        std::shared_ptr<WindowHandle> getHandle() {
            return this->windowHandle;
        }
    };

}
