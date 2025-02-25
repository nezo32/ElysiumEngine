#pragma once

#include <memory>

#include "device/device.hpp"
#include "render_pass.hpp"
#include "swap_chain.hpp"

namespace Ely {

class FrameBuffer {
   private:
    Device& device;

    std::vector<VkFramebuffer> frameBuffers;

   public:
    FrameBuffer(Device& d, std::unique_ptr<SwapChain>& swapChain, RenderPass& renderPass);
    ~FrameBuffer();

    std::vector<VkFramebuffer> GetFrameBuffers() { return frameBuffers; }
};

}   // namespace Ely