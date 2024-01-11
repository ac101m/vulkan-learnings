#include "utils/glfw/helpers.hpp"
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
#include <map>


class Application {
private:
    static utils::Logger log;

    std::shared_ptr<utils::glfw::Window> glfwWindow;
    std::shared_ptr<utils::vulkan::Instance> vkInstance;
    std::shared_ptr<utils::vulkan::Surface> vkPresentSurface;
    std::shared_ptr<utils::vulkan::PhysicalDevice> vkPhysicalDevice;
    std::shared_ptr<utils::vulkan::Device> vkDevice;
    std::shared_ptr<utils::vulkan::Queue> vkGraphicsQueue;
    std::shared_ptr<utils::vulkan::Queue> vkPresentQueue;

    std::vector<std::string> const debugValidationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };

    std::string const graphicsQueueName = "GRAPHICS_QUEUE";
    std::string const presentQueueName = "PRESENT_QUEUE";

    utils::vulkan::QueuePlan createQueuePlan() const {
        utils::vulkan::QueuePlan queuePlan;

        if (this->vkPresentSurface == nullptr) {
            throw std::runtime_error("Cannot create queue plan, present surface is not initialized.");
        }

        auto const graphicsQueueConstraints = utils::vulkan::QueueConstraints(VK_QUEUE_GRAPHICS_BIT, nullptr);
        auto const presentQueueConstraints = utils::vulkan::QueueConstraints(0, this->vkPresentSurface);

        queuePlan.addQueue(graphicsQueueName, graphicsQueueConstraints);
        queuePlan.addQueue(presentQueueName, presentQueueConstraints);

        return queuePlan;
    }


public:
    Application(uint32_t const windowWidth, uint32_t const windowHeight, bool doDebug) {
        auto const validationLayers = doDebug ? debugValidationLayers : std::vector<std::string>(0);

        this->glfwWindow = std::make_shared<utils::glfw::Window>("Vulkan learnings", windowWidth, windowHeight);
        this->vkInstance = std::make_shared<utils::vulkan::Instance>(validationLayers);
        this->vkPresentSurface = this->vkInstance->createWindowSurface(this->glfwWindow);

        auto const queuePlan = createQueuePlan();

        this->vkPhysicalDevice = this->vkInstance->selectPhysicalDevice(queuePlan);

        INFO(log) << "Selected physical device '" << this->vkPhysicalDevice->getProperties().deviceName << '\'' << std::endl;

        this->vkDevice = this->vkPhysicalDevice->createLogicalDevice(queuePlan);
        this->vkGraphicsQueue = this->vkDevice->getQueue(graphicsQueueName);
        this->vkPresentQueue = this->vkDevice->getQueue(presentQueueName);
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
