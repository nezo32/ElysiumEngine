#pragma once

#include <memory>

#include "external/glfw.hpp"

namespace Ely {

struct ElysiumDependencies;

class CommandBuffer {
   private:
    VkCommandBuffer commandBuffer;
    ElysiumDependencies& deps;

   public:
    CommandBuffer(ElysiumDependencies& deps, VkCommandBuffer buffer = nullptr);

    void Reset(VkCommandBufferResetFlags flags = 0);
    void Submit(VkSemaphore* waitSemaphores, VkSemaphore* signalSemaphores, VkFence fence,
                uint32_t waitSemaphoresCount = 1, uint32_t signalSemaphoresCount = 1);
    void BeginRecord();
    void BeginRenderPass(uint32_t imageIndex);
    void EndRecord();
    void EndRenderPass();

    VkCommandBuffer GetCommandBuffer() { return commandBuffer; }
};

}   // namespace Ely