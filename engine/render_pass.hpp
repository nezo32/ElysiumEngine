#pragma once

#include <memory>

#include "device/device.hpp"
#include "external/glfw.hpp"
#include "swap_chain.hpp"

namespace Ely {
class RenderPass {
   private:
    VkRenderPass renderPass;

    Device &device;

   public:
    RenderPass(Device &d, std::unique_ptr<SwapChain> &swapChain);
    ~RenderPass();

    VkRenderPass GetRenderPass() { return renderPass; }
};
}   // namespace Ely