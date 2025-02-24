#pragma once

#include <memory>

#include "command_pool.hpp"
#include "device/device.hpp"
#include "external/glfw.hpp"
#include "frame_buffer.hpp"
#include "pipeline/pipeline.hpp"
#include "render_pass.hpp"
#include "swap_chain.hpp"

namespace Ely {

class CommandBuffer {
   private:
    RenderPass& renderPass;
    std::unique_ptr<FrameBuffer>& frameBuffer;
    std::unique_ptr<SwapChain>& swapChain;
    Device& device;
    VkCommandBuffer commandBuffer;
    Pipeline& pipeline;

   public:
    CommandBuffer(Device& device, CommandPool& commandPool, std::unique_ptr<SwapChain>& swapChain,
                  RenderPass& renderPass, std::unique_ptr<FrameBuffer>& frameBuffer, Pipeline& pipeline,
                  VkCommandBuffer buffer = nullptr);

    void Reset(VkCommandBufferResetFlags flags = 0);
    void Submit(VkSemaphore* waitSemaphores, VkSemaphore* signalSemaphores, VkFence fence,
                uint32_t waitSemaphoresCount = 1, uint32_t signalSemaphoresCount = 1);
    void Record(uint32_t imageIndex);

    VkCommandBuffer GetCommandBuffer() { return commandBuffer; }
};

}   // namespace Ely