#include "utils/glfw/helpers.hpp"
#include "utils/glfw/window.hpp"
#include "utils/vulkan/instance.hpp"
#include "utils/vulkan/device.hpp"
#include "utils/vulkan/swap_chain.hpp"
#include "utils/vulkan/render_pass.hpp"
#include "utils/misc/logging.hpp"
#include "utils/misc/file.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <map>


struct ColorVertex {
    glm::vec2 position;
    glm::vec3 color;

    static void addToConfig(utils::vulkan::VertexInfo * const vertexInfo) {
        uint32_t index = vertexInfo->addVertexType(sizeof(ColorVertex), VK_VERTEX_INPUT_RATE_VERTEX);
        vertexInfo->addVertexAttribute(index, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(ColorVertex, position));
        vertexInfo->addVertexAttribute(index, 1, VK_FORMAT_R32G32B32_SFLOAT, offsetof(ColorVertex, color));
    }
};


std::vector<ColorVertex> const triangleVertices = {
    {{ 0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    {{ 0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}},
    {{-0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}}
};


class Application {
private:
    static utils::Logger log;

    std::shared_ptr<utils::glfw::Window> glfwWindow;
    std::shared_ptr<utils::vulkan::Instance> vkInstance;
    std::shared_ptr<utils::vulkan::Surface> vkPresentSurface;

    std::shared_ptr<utils::vulkan::PhysicalDevice> vkPhysicalDevice;
    std::shared_ptr<utils::vulkan::Device> vkDevice;
    std::shared_ptr<utils::vulkan::Queue> vkGraphicsQueue;

    std::shared_ptr<utils::vulkan::SwapChain> vkSwapChain;
    std::vector<std::shared_ptr<utils::vulkan::ImageView>> vkSwapChainImageViews;
    std::vector<std::shared_ptr<utils::vulkan::FrameBuffer>> vkFrameBuffers;

    std::shared_ptr<utils::vulkan::ShaderModule> vkVertexShaderModule;
    std::shared_ptr<utils::vulkan::ShaderModule> vkFragmentShaderModule;
    std::shared_ptr<utils::vulkan::PipelineLayout> vkPipelineLayout;
    std::shared_ptr<utils::vulkan::RenderPass> vkRenderPass;
    std::shared_ptr<utils::vulkan::GraphicsPipeline> vkGraphicsPipeline;
    std::shared_ptr<utils::vulkan::CommandPool> vkCommandPool;

    std::vector<std::string> const debugValidationLayers = {
        "VK_LAYER_KHRONOS_validation"
    };

    std::vector<std::string> const requiredDeviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    std::string const graphicsQueueName = "GRAPHICS_QUEUE";

    uint32_t const MAX_FRAMES_IN_FLIGHT = 2;


    utils::vulkan::QueuePlan createQueuePlan() const {
        utils::vulkan::QueuePlan queuePlan;

        if (this->vkPresentSurface == nullptr) {
            throw std::runtime_error("Cannot create queue plan, present surface is not initialized.");
        }

        auto const graphicsQueueConstraints = utils::vulkan::QueueConstraints(VK_QUEUE_GRAPHICS_BIT, this->vkPresentSurface);

        queuePlan.addQueue(graphicsQueueName, graphicsQueueConstraints);

        return queuePlan;
    }


    utils::vulkan::SwapChainPreferences createSwapChainPreferences() const {
        auto prefs = utils::vulkan::SwapChainPreferences();

        prefs.addSurfaceFormat({VK_FORMAT_B8G8R8A8_SRGB, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR});
        prefs.addPresentMode(VK_PRESENT_MODE_FIFO_KHR);

        return prefs;
    }


    /**
     * @brief Calculate a suitable swap chain image count.
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

        auto const swapChainSupportInfo = this->vkPhysicalDevice->getSwapChainSupportInfo(this->vkPresentSurface);
        auto const swapChainPreferences = createSwapChainPreferences();

        config.presentMode = swapChainPreferences.selectPresentMode(swapChainSupportInfo.supportedPresentModes);
        config.surfaceFormat = swapChainPreferences.selectSurfaceFormat(swapChainSupportInfo.supportedSurfaceFormats);
        config.imageCount = pickSwapChainImageCount(swapChainSupportInfo);
        config.imageExtent = determineSwapChainImageExtent(swapChainSupportInfo.capabilities);
        config.queueFamilyIndices = {this->vkGraphicsQueue->queueFamilyIndex};
        config.preTransform = swapChainSupportInfo.capabilities.currentTransform;

        return config;
    }


    utils::vulkan::ImageViewConfig createSwapChainImageViewConfig() {
        utils::vulkan::ImageViewConfig config;

        config.imageFormat = this->vkSwapChain->config.surfaceFormat.format;
        config.viewType = VK_IMAGE_VIEW_TYPE_2D;

        return config;
    }


    utils::vulkan::RenderPassConfig createRenderPassConfig() {
        utils::vulkan::RenderPassConfig config;

        utils::vulkan::AttachmentDescription output;
        output.format = this->vkSwapChain->config.surfaceFormat.format;
        output.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        output.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        output.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        uint32_t outputIndex = config.addAttachment(output);

        utils::vulkan::SubPassDescription subPass(VK_PIPELINE_BIND_POINT_GRAPHICS);
        subPass.addColorAttachment(outputIndex, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);

        uint32_t subPassIndex = config.addSubPass(subPass);
        config.addSubPassDependency(
            VK_SUBPASS_EXTERNAL, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, subPassIndex,
            subPassIndex, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT
        );

        return config;
    }


    utils::vulkan::GraphicsPipelineConfig createGraphicsPipelineConfig() {
        auto config = utils::vulkan::GraphicsPipelineConfig();

        config.addShaderStage(this->vkVertexShaderModule->getHandle(), VK_SHADER_STAGE_VERTEX_BIT);
        config.addShaderStage(this->vkFragmentShaderModule->getHandle(), VK_SHADER_STAGE_FRAGMENT_BIT);

        ColorVertex::addToConfig(&config.vertexInfo);

        return config;
    }


    utils::vulkan::CommandPoolConfig createCommandPoolConfig() {
        auto config = utils::vulkan::CommandPoolConfig(this->vkGraphicsQueue->queueFamilyIndex);
        config.flagBits = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        return config;
    }


    void recreateSwapChain() {
        INFO(log) << "Re-creating swap chain..." << std::endl;

        auto frameBufferSize = this->glfwWindow->getFrameBufferSize();
        while (frameBufferSize.width == 0 || frameBufferSize.height == 0) {
            frameBufferSize = this->glfwWindow->getFrameBufferSize();
            glfwWaitEvents();
        }

        this->vkDevice->waitIdle();

        this->vkFrameBuffers.clear();
        this->vkSwapChainImageViews.clear();
        this->vkSwapChain.reset();

        this->vkSwapChain = this->vkDevice->createSwapChain(this->vkPresentSurface, buildSwapChainConfig());
        this->vkSwapChainImageViews = this->vkSwapChain->createImageViews(createSwapChainImageViewConfig());

        for (unsigned i = 0; i < this->vkSwapChainImageViews.size(); i++) {
            utils::vulkan::FrameBufferConfig config(this->vkSwapChain->config.imageExtent);
            config.addAttachment(this->vkSwapChainImageViews[i]->getHandle());
            this->vkFrameBuffers.push_back(this->vkDevice->createFrameBuffer(this->vkRenderPass, config));
        }

        INFO(log) << "Swap chain recreated." << std::endl;
    }


public:
    Application(uint32_t const windowWidth, uint32_t const windowHeight, bool doDebug) {
        auto const validationLayers = doDebug ? debugValidationLayers : std::vector<std::string>(0);

        this->glfwWindow = std::make_shared<utils::glfw::Window>("Vulkan learnings", windowWidth, windowHeight, true);
        this->vkInstance = std::make_shared<utils::vulkan::Instance>(validationLayers);
        this->vkPresentSurface = this->vkInstance->createWindowSurface(this->glfwWindow);

        auto const queuePlan = createQueuePlan();

        this->vkPhysicalDevice = this->vkInstance->selectPhysicalDevice(queuePlan, requiredDeviceExtensions);

        INFO(log) << "Selected physical device '" << this->vkPhysicalDevice->getProperties().deviceName << '\'' << std::endl;

        this->vkDevice = this->vkPhysicalDevice->createLogicalDevice(queuePlan, requiredDeviceExtensions);

        this->vkGraphicsQueue = this->vkDevice->getQueue(graphicsQueueName);
        this->vkSwapChain = this->vkDevice->createSwapChain(this->vkPresentSurface, buildSwapChainConfig());
        this->vkSwapChainImageViews = this->vkSwapChain->createImageViews(createSwapChainImageViewConfig());

        this->vkVertexShaderModule = this->vkDevice->createShaderModule("data/shaders/triangle/vertex.spv");
        this->vkFragmentShaderModule = this->vkDevice->createShaderModule("data/shaders/triangle/fragment.spv");
        this->vkPipelineLayout = this->vkDevice->createPipelineLayout({});
        this->vkRenderPass = this->vkDevice->createRenderPass(createRenderPassConfig());

        this->vkGraphicsPipeline = this->vkDevice->createGraphicsPipeline(
            this->vkPipelineLayout,
            this->vkRenderPass,
            createGraphicsPipelineConfig());

        for (unsigned i = 0; i < this->vkSwapChainImageViews.size(); i++) {
            utils::vulkan::FrameBufferConfig config(this->vkSwapChain->config.imageExtent);
            config.addAttachment(this->vkSwapChainImageViews[i]->getHandle());
            this->vkFrameBuffers.push_back(this->vkDevice->createFrameBuffer(this->vkRenderPass, config));
        }

        this->vkCommandPool = this->vkDevice->createCommandPool(createCommandPoolConfig());
    }


    void run() {
        INFO(log) << "Main loop starting." << std::endl;

        unsigned contextIndex = 0;

        std::vector<std::shared_ptr<utils::vulkan::CommandBuffer>> commandBuffers(MAX_FRAMES_IN_FLIGHT);
        std::vector<std::shared_ptr<utils::vulkan::Semaphore>> imageAvailableSemaphores(MAX_FRAMES_IN_FLIGHT);
        std::vector<std::shared_ptr<utils::vulkan::Semaphore>> renderCompleteSemaphores(MAX_FRAMES_IN_FLIGHT);
        std::vector<std::shared_ptr<utils::vulkan::Fence>> inFlightFences(MAX_FRAMES_IN_FLIGHT);

        for (unsigned i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            commandBuffers[i] = this->vkCommandPool->allocateCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY);
            imageAvailableSemaphores[i] = this->vkDevice->createSemaphore();
            renderCompleteSemaphores[i] = this->vkDevice->createSemaphore();
            inFlightFences[i] = this->vkDevice->createFence(VK_FENCE_CREATE_SIGNALED_BIT);
        }

        std::shared_ptr<utils::vulkan::VertexBuffer> vertexBuffer = this->vkDevice->createVertexBuffer(
            triangleVertices.size() * sizeof(ColorVertex), VK_SHARING_MODE_EXCLUSIVE);

        VkMemoryRequirements memoryRequirements = vertexBuffer->getMemoryRequirements();

        while (!glfwWindow->shouldClose()) {
            glfwPollEvents();

            auto const& commandBuffer = commandBuffers[contextIndex];
            auto const& imageAvailableSemaphore = imageAvailableSemaphores[contextIndex];
            auto const& renderCompleteSemaphore = renderCompleteSemaphores[contextIndex];
            auto const& inFlightFence = inFlightFences[contextIndex];

            contextIndex = (contextIndex + 1) % MAX_FRAMES_IN_FLIGHT;

            // Wait for the previous frame to be done
            inFlightFence->wait();

            // Get an image from the swap chain
            VkResult result;
            uint32_t const nextImageIndex = this->vkSwapChain->getNextImage(imageAvailableSemaphore, &result);

            if (result == VK_ERROR_OUT_OF_DATE_KHR) {
                recreateSwapChain();
                continue;
            } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
                throw std::runtime_error("Failed to acquire swap chain image.");
            }

            // Reset the fence in preparation for doing work
            inFlightFence->reset();

            // Record the command buffer
            commandBuffer->reset();
            commandBuffer->begin();
            commandBuffer->beginRenderPass(
                this->vkRenderPass,
                this->vkFrameBuffers[nextImageIndex],
                {0, 0},
                this->vkSwapChain->config.imageExtent);
            commandBuffer->bindGraphicsPipeline(this->vkGraphicsPipeline);
            commandBuffer->setViewport(this->vkSwapChain->config.imageExtent);
            commandBuffer->setScissor({0, 0}, this->vkSwapChain->config.imageExtent);
            commandBuffer->draw(3, 1, 0, 0);
            commandBuffer->endRenderPass();
            commandBuffer->end();

            // Submit the command buffer to render some stuff
            this->vkGraphicsQueue->submit(
                {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT},
                {imageAvailableSemaphore},
                {renderCompleteSemaphore},
                {commandBuffer},
                inFlightFence);

            // Present the rendered image!
            this->vkGraphicsQueue->present(
                {renderCompleteSemaphore},
                this->vkSwapChain,
                nextImageIndex,
                &result);

            if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || this->glfwWindow->hasResized()) {
                this->glfwWindow->resetResized();
                recreateSwapChain();
            } else if (result != VK_SUCCESS) {
                throw std::runtime_error("Failed to present swap chain image.");
            }
        }

        INFO(log) << "Main loop stopped, waiting for device idle..." << std::endl;

        this->vkDevice->waitIdle();

        INFO(log) << "Device idle, shutting down." << std::endl;
    }
};


utils::Logger Application::log = utils::Logger("Application");


int main(void) {
    utils::Logger logger = utils::Logger("Main");

    utils::glfw::Initializer glfwInitializer;

#ifdef NDEBUG
    bool doDebug = false;
#else
    bool doDebug = true;
#endif

    try {
        auto application = Application(800, 450, doDebug);
        application.run();
    } catch (const std::exception& e) {
        ERROR(logger) << "Fatal error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
