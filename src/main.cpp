#include "utils/glfw/initializer.hpp"
#include "utils/glfw/window.hpp"
#include "utils/vulkan/instance.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>
#include <vector>
#include <memory>


class Application {
private:
    std::shared_ptr<utils::glfw::Window> window;
    std::shared_ptr<utils::vulkan::Instance> vkInstance;

    std::vector<std::string> const debugValidationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };

public:
    Application(uint32_t const windowWidth, uint32_t const windowHeight, bool doDebug) {
        auto const validationLayers = doDebug ? debugValidationLayers : std::vector<std::string>(0);

        this->window = std::shared_ptr<utils::glfw::Window>(new utils::glfw::Window(windowWidth, windowHeight));
        this->vkInstance = std::shared_ptr<utils::vulkan::Instance>(new utils::vulkan::Instance(validationLayers));
    }

    void run() {
        while (!window->shouldClose()) {
            glfwPollEvents();
        }
    }
};


int main(void) {
    utils::glfw::Initializer glfwInitializer;

    try {
        auto application = Application(640, 480, true);
        application.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
