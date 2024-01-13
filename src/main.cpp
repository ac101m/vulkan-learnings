#include "utils/glfw/helpers.hpp"
#include "utils/glfw/window.hpp"
#include "utils/vulkan/instance.hpp"
#include "utils/vulkan/device.hpp"
#include "utils/vulkan/swap_chain.hpp"
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

    std::shared_ptr<utils::vulkan::SwapChain> vkSwapChain;
    std::vector<std::shared_ptr<utils::vulkan::ImageView>> vkSwapChainImageViews;

    std::vector<std::string> const debugValidationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };

    std::vector<std::string> const requiredDeviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
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


    utils::vulkan::SwapChainPreferences createSwapChainPreferences() const {
        auto prefs = utils::vulkan::SwapChainPreferences();

        prefs.addSurfaceFormat({VK_FORMAT_B8G8R8A8_SRGB, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR});
        prefs.addPresentMode(VK_PRESENT_MODE_FIFO_KHR);

        return prefs;
    }


    /**
     * @brief Calculate a suitable swap chain image cound.
     * We want one more image that the minimum supported by the swap chain,
     * but we don't want to exceed the safe upper limit.
     */
    uint32_t const pickSwapChainImageCount(utils::vulkan::SwapChainSupportInfo const& info) const {
        uint32_t count = info.capabilities.minImageCount + 1;

        if (info.capabilities.maxImageCount != 0 && count > info.capabilities.maxImageCount) {
            count = info.capabilities.maxImageCount;
        }

        return count;
    }


    /**
     * @brief Calculate suitable swap chain extent.
     */
    VkExtent2D determineSwapChainImageExtent(VkSurfaceCapabilitiesKHR const& capabilities) const {
        if (this->glfwWindow == nullptr) {
            throw std::runtime_error("Cannot calculate swap chain image extent, window is not yet initialized.");
        }

        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
            return capabilities.currentExtent;
        } else {
            VkExtent2D actualExtent = this->glfwWindow->getFrameBufferSize();

            actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
            actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

            return actualExtent;
        }

        return {0, 0};
    }


    utils::vulkan::SwapChainConfig buildSwapChainConfig() const {
        utils::vulkan::SwapChainConfig config;

        if (this->vkPhysicalDevice == nullptr) {
            throw std::runtime_error("Cannot create swap chain config, physical device is not yet initialized.");
        }

        if (this->vkPresentSurface == nullptr) {
            throw std::runtime_error("Cannot create swap chain config, present surface is not initialized.");
        }

        if (this->vkGraphicsQueue == nullptr) {
            throw std::runtime_error("Cannot create swap chain config, graphics queue is not initialized.");
        }

        if (this->vkPresentQueue == nullptr) {
            throw std::runtime_error("Cannot create swap chain config, present queue is not initialized.");
        }

        auto const swapChainSupportInfo = this->vkPhysicalDevice->getSwapChainSupportInfo(this->vkPresentSurface);
        auto const swapChainPreferences = createSwapChainPreferences();

        config.presentMode = swapChainPreferences.selectPresentMode(swapChainSupportInfo.supportedPresentModes);
        config.surfaceFormat = swapChainPreferences.selectSurfaceFormat(swapChainSupportInfo.supportedSurfaceFormats);
        config.imageCount = pickSwapChainImageCount(swapChainSupportInfo);
        config.imageExtent = determineSwapChainImageExtent(swapChainSupportInfo.capabilities);
        config.queueFamilyIndices = {this->vkGraphicsQueue->queueFamilyIndex, this->vkPresentQueue->queueFamilyIndex};
        config.preTransform = swapChainSupportInfo.capabilities.currentTransform;

        return config;
    }


public:
    Application(uint32_t const windowWidth, uint32_t const windowHeight, bool doDebug) {
        auto const validationLayers = doDebug ? debugValidationLayers : std::vector<std::string>(0);

        this->glfwWindow = std::make_shared<utils::glfw::Window>("Vulkan learnings", windowWidth, windowHeight);
        this->vkInstance = std::make_shared<utils::vulkan::Instance>(validationLayers);
        this->vkPresentSurface = this->vkInstance->createWindowSurface(this->glfwWindow);

        auto const queuePlan = createQueuePlan();

        this->vkPhysicalDevice = this->vkInstance->selectPhysicalDevice(queuePlan, requiredDeviceExtensions);

        INFO(log) << "Selected physical device '" << this->vkPhysicalDevice->getProperties().deviceName << '\'' << std::endl;

        this->vkDevice = this->vkPhysicalDevice->createLogicalDevice(queuePlan, requiredDeviceExtensions);

        this->vkGraphicsQueue = this->vkDevice->getQueue(graphicsQueueName);
        this->vkPresentQueue = this->vkDevice->getQueue(presentQueueName);

        auto const swapChainConfig  = buildSwapChainConfig();

        this->vkSwapChain = this->vkDevice->createSwapChain(this->vkPresentSurface, swapChainConfig);

        utils::vulkan::ImageViewConfig swapChainImageViewConfig;
        swapChainImageViewConfig.imageFormat = this->vkSwapChain->config.surfaceFormat.format;
        swapChainImageViewConfig.viewType = VK_IMAGE_VIEW_TYPE_2D;

        for (auto const& image : this->vkSwapChain->getImages()) {
            vkSwapChainImageViews.push_back(image->createImageView(swapChainImageViewConfig));
        }
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
        auto application = Application(800, 450, true);
        application.run();
    } catch (const std::exception& e) {
        ERROR(logger) << "Fatal error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
