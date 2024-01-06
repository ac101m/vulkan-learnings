#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


namespace utils::glfw {

    /**
     * @Brief Class for managing GLFW library lifecycle.
     */
    class Initializer {
    public:
        Initializer() {
            glfwInit();
        }

        ~Initializer() {
            glfwTerminate();
        }
    };

}
