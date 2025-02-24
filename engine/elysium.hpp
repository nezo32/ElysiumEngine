#include <memory>

#include "command/command_buffer.hpp"
#include "command/command_pool.hpp"
#include "device/device.hpp"
#include "events.hpp"
#include "frame_buffer.hpp"
#include "pipeline/pipeline.hpp"
#include "pipeline/pipeline_layout.hpp"
#include "render_pass.hpp"
#include "renderer.hpp"
#include "utils/debug_log.hpp"
#include "utils/sync.hpp"
#include "window.hpp"

namespace Ely {

class Elysium {
   private:
    int width;
    int height;

   public:
    Elysium(const int windowWidth, const int windowHeight, const char* windowTitle, const char* appName,
            const uint32_t appVersion)
        : window{windowWidth, windowHeight, windowTitle}, vulkan{window, appName, appVersion} {
        width = windowWidth;
        height = windowHeight;
    }

    Window window;
    Vulkan vulkan;
    PhysDevice physDevice{vulkan};
    Device device{vulkan, physDevice};
    std::unique_ptr<SwapChain> swapChain{std::make_unique<SwapChain>(window, vulkan, physDevice, device)};
    RenderPass renderPass{device, swapChain};
    PipelineLayout pipelineLayout{device};
    Pipeline pipeline{
        device, Pipeline::defaultPipelineConfigInfo(renderPass.GetRenderPass(), pipelineLayout.GetPipelineLayout()),
        "vertex_core.vert.spv", "fragment_core.frag.spv"};
    std::unique_ptr<FrameBuffer> frameBuffer{std::make_unique<FrameBuffer>(device, swapChain, renderPass)};
    CommandPool commandPool{vulkan, physDevice, device};
    Renderer renderer{window, vulkan, physDevice, device, commandPool, swapChain, renderPass, frameBuffer, pipeline};

    static uint32_t GetVersionCode(uint32_t major, uint32_t minor, uint32_t patch) {
        return VK_MAKE_VERSION(major, minor, patch);
    }
};
}   // namespace Ely