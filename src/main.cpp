#include "utils/glfw/initializer.hpp"
#include "utils/glfw/window.hpp"
#include "utils/vulkan/instance.hpp"
#include "utils/misc/logging.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>


class Application {
private:
    utils::Logger log = utils::Logger("Application");

    std::shared_ptr<utils::glfw::Window> window;
    std::shared_ptr<utils::vulkan::Instance> vkInstance;

    std::vector<std::string> const debugValidationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };


    utils::vulkan::PhysicalDevice selectPhysicalDevice() const {
        auto const devices = this->vkInstance->getPhysicalDevices();

        uint32_t highScore = 0;
        utils::vulkan::PhysicalDevice const * selectedDevice;

        for (auto const& device : devices) {
            if (device.getScore() > highScore) {
                highScore = device.getScore();
                selectedDevice = &device;
            }
        }

        if (highScore == 0) {
            throw std::runtime_error("No suitable vulkan devices.");
        }

        return *selectedDevice;
    }


public:
    Application(uint32_t const windowWidth, uint32_t const windowHeight, bool doDebug) {
        auto const validationLayers = doDebug ? debugValidationLayers : std::vector<std::string>(0);

        this->vkInstance = std::shared_ptr<utils::vulkan::Instance>(new utils::vulkan::Instance(validationLayers));

        auto const device = selectPhysicalDevice();

        INFO(log) << "Selected GPU: '" << device.getProperties().deviceName << "'" << std::endl;

        this->window = std::shared_ptr<utils::glfw::Window>(new utils::glfw::Window("Vulkan learnings", windowWidth, windowHeight));
    }


    void run() {
        INFO(log) << "Main loop starting." << std::endl;

        while (!window->shouldClose()) {
            glfwPollEvents();
        }

        INFO(log) << "Main loop stopped." << std::endl;
    }
};


int main(void) {
    utils::Logger logger = utils::Logger("Main");

    utils::glfw::Initializer glfwInitializer;

    try {
        auto application = Application(640, 480, true);
        application.run();
    } catch (const std::exception& e) {
        ERROR(logger) << "Fatal error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
