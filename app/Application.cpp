#include "Application.hpp"

#include <array>

void Application::Run() {
    while (!elyWindow.ShouldClose()) {
        Ely::ElyEvents::PollEvents();
        drawFrame();
    }
    vkDeviceWaitIdle(elyDevice.device());
}

Application::Application() {
    createPipelineLayout();
    createPipeline();
    createCommandBuffers();
}

Application::~Application() { vkDestroyPipelineLayout(elyDevice.device(), pipelineLayout, nullptr); }

void Application::createPipeline() {
    auto pipelineConfig = Ely::ElyPipeline::defaultPipelineConfigInfo(elySwapChain.width(), elySwapChain.height());
    pipelineConfig.renderPass = elySwapChain.getRenderPass();
    pipelineConfig.pipelineLayout = pipelineLayout;
    elyPipeline =
        std::make_unique<Ely::ElyPipeline>(elyDevice, pipelineConfig, "vertex_core.vert.spv", "fragment_core.frag.spv");
}

void Application::createPipelineLayout() {
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;

    if (vkCreatePipelineLayout(elyDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
        throw std::runtime_error("ERROR :: Application :: Failed to create pipeline layout");
    }
}

void Application::createCommandBuffers() {
    commandBuffers.resize(elySwapChain.imageCount());
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = elyDevice.getCommandPool();
    allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

    if (vkAllocateCommandBuffers(elyDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("ERROR :: Application :: Failed to allocate command buffers");
    }

    for (int i = 0; i < commandBuffers.size(); i++) {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("ERROR :: Application :: failed to begin recording command buffers");
        }

        VkRenderPassBeginInfo passInfo{};
        passInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        passInfo.renderPass = elySwapChain.getRenderPass();
        passInfo.framebuffer = elySwapChain.getFrameBuffer(i);

        passInfo.renderArea.offset = {0, 0};
        passInfo.renderArea.extent = elySwapChain.getSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {0.f, 0.f, 0.f, 1.f};
        clearValues[1].depthStencil = {1.f, 0};

        passInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        passInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffers[i], &passInfo, VK_SUBPASS_CONTENTS_INLINE);

        elyPipeline->Bind(commandBuffers[i]);
        vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);

        vkCmdEndRenderPass(commandBuffers[i]);

        if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("ERROR :: Application :: Failed to record command buffer");
        }
    }
}

void Application::drawFrame() {
    uint32_t imageIndex;
    auto result = elySwapChain.acquireNextImage(&imageIndex);

    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("ERROR :: Application :: failed to acquire swap chain image");
    }

    result = elySwapChain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
    if (result != VK_SUCCESS) {
        throw std::runtime_error("ERROR :: Application :: failed to present swap chain image");
    }
}