#include "utils/glfw/initializer.hpp"
#include "utils/glfw/window.hpp"
#include "utils/vulkan/instance.hpp"
#include "utils/vulkan/device.hpp"
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
    static utils::Logger log;

    std::shared_ptr<utils::glfw::Window> glfwWindow;
    std::shared_ptr<utils::vulkan::Instance> vkInstance;
    std::shared_ptr<utils::vulkan::PhysicalDevice> vkPhysicalDevice;
    std::shared_ptr<utils::vulkan::Device> vkDevice;

    std::vector<std::string> const debugValidationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };

    uint32_t const deviceQueueFlags = VK_QUEUE_GRAPHICS_BIT;


public:
    Application(uint32_t const windowWidth, uint32_t const windowHeight, bool doDebug) {
        auto const validationLayers = doDebug ? debugValidationLayers : std::vector<std::string>(0);

        this->vkInstance = std::make_shared<utils::vulkan::Instance>(validationLayers);
        this->vkPhysicalDevice = this->vkInstance->selectPhysicalDevice(deviceQueueFlags);

        INFO(log) << "Selected physical device '" << this->vkPhysicalDevice->getProperties().deviceName << '\'' << std::endl;

        auto logicalDevice = this->vkPhysicalDevice->createLogicalDevice(deviceQueueFlags, 1);

        this->glfwWindow = std::make_shared<utils::glfw::Window>("Vulkan learnings", windowWidth, windowHeight);
    }


    void run() {
        INFO(log) << "Main loop starting." << std::endl;

        while (!glfwWindow->shouldClose()) {
            glfwPollEvents();
        }

        INFO(log) << "Main loop stopped." << std::endl;
    }
};


utils::Logger Application::log = utils::Logger("Application");


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
