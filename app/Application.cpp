#include "Application.hpp"

void Application::Run() {
    while (!elyWindow.ShouldClose()) {
        Ely::ElyEvents::PollEvents();
    }
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

void Application::createCommandBuffers() {}

void Application::drawFrame() {}