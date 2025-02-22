#pragma once

#include "ely_device.hpp"
#include "ely_events.hpp"
#include "ely_phys_device.hpp"
#include "ely_pipeline.hpp"
#include "ely_vulkan.hpp"
#include "ely_window.hpp"

namespace Ely {
class Elysium {
   public:
    Elysium(const int windowWidth, const int windowHeight, const char* windowTitle, const char* appName,
            uint32_t appVersion)
        : elyWindow{windowWidth, windowHeight, windowTitle}, elyVulkan{appName, appVersion} {};

    ElyWindow elyWindow;
    ElyVulkan elyVulkan;
    ElyPhysDevice elyPhysDevice{elyVulkan};
    ElyDevice elyDevice{elyPhysDevice};
    ElyPipeline elyPipeline{elyDevice,
                            ElyPipeline::defaultPipelineConfigInfo(elyWindow.GetWidth(), elyWindow.GetHeight()),
                            "vertex_core.vert.spv", "fragment_core.frag.spv"};
};
}   // namespace Ely