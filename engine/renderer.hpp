#pragma once

#include <memory>
#include <vector>

#include "buffer/vertex_buffer.hpp"
#include "command/command_buffer.hpp"
#include "device/phys_device.hpp"
#include "external/glfw.hpp"
#include "utils/sync.hpp"
#include "vulkan.hpp"
#include "window.hpp"

namespace Ely {

class Renderer {
   private:
    const int MAX_FRAMES_IN_FLIGHT = 2;
    size_t currentFrame = 0;

    RenderPass& renderPass;
    Vulkan& vulkan;
    PhysDevice& physDevice;
    Device& device;
    Window& window;
    std::unique_ptr<FrameBuffer>& frameBuffer;
    std::unique_ptr<SwapChain>& swapChain;
    VkCommandBuffer commandBuffer;

    // TODO: implement model/vertex uploading
    std::vector<Vertex> vertices = {{{0.0f, -0.5f, 0.f}, {1.0f, 0.0f, 0.0f}},
                                    {{0.5f, 0.5f, 0.f}, {1.0f, 1.0f, 1.0f}},
                                    {{-0.5f, 0.5f, 0.f}, {0.0f, 0.0f, 1.0f}}};
    VertexBuffer vertexBuffer{physDevice, device, vertices.size()};

    std::vector<VkCommandBuffer> buffers;
    std::vector<std::unique_ptr<CommandBuffer>> commandBuffers;
    std::vector<std::unique_ptr<Sync::Semaphore>> imageAvailableSemaphores;
    std::vector<std::unique_ptr<Sync::Semaphore>> renderFinishedSemaphores;
    std::vector<std::unique_ptr<Sync::Fence>> inFlightFences;

    void present(VkSemaphore* signal, uint32_t* imageIndices);
    void recreate();

   public:
    Renderer(Window& window, Vulkan& vulkan, PhysDevice& physDevice, Device& device, CommandPool& commandPool,
             std::unique_ptr<SwapChain>& swapChain, RenderPass& renderPass, std::unique_ptr<FrameBuffer>& frameBuffer,
             Pipeline& pipeline);

    void Draw();
};

}   // namespace Ely