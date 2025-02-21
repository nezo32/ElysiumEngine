#pragma once

#include <memory>
#include <vector>

#include "elysium/ely_device.h"
#include "elysium/ely_events.h"
#include "elysium/ely_pipeline.h"
#include "elysium/ely_swap_chain.h"
#include "elysium/ely_window.h"

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