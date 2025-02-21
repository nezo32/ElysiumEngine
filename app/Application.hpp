#pragma once

#include <memory>
#include <vector>

#include "elysium/ely_device.hpp"
#include "elysium/ely_events.hpp"
#include "elysium/ely_pipeline.hpp"
#include "elysium/ely_swap_chain.hpp"
#include "elysium/ely_window.hpp"

class Application {
   public:
    void Run();

    Application();
    ~Application();

    Application(const Application &) = delete;
    Application &operator=(const Application &) = delete;

   private:
    void createPipelineLayout();
    void createPipeline();
    void createCommandBuffers();
    void drawFrame();

    static constexpr int WIDTH = 1280;
    static constexpr int HEIGHT = 720;
    static constexpr const char *APP_NAME = "ZALUPA";

    Ely::ElyWindow elyWindow{WIDTH, HEIGHT, APP_NAME};
    Ely::ElyDevice elyDevice{elyWindow};
    Ely::ElySwapChain elySwapChain{elyDevice, elyWindow.GetExtent()};
    std::unique_ptr<Ely::ElyPipeline> elyPipeline;
    VkPipelineLayout pipelineLayout;
    std::vector<VkCommandBuffer> commandBuffers;
};