#include "renderer.hpp"

#include "events.hpp"

namespace Ely {

Renderer::Renderer(Window& window, Vulkan& vulkan, PhysDevice& physDevice, Device& device, CommandPool& commandPool,
                   std::unique_ptr<SwapChain>& swapChain, RenderPass& renderPass,
                   std::unique_ptr<FrameBuffer>& frameBuffer, Pipeline& pipeline)
    : vulkan{vulkan},
      physDevice{physDevice},
      window{window},
      device{device},
      swapChain{swapChain},
      renderPass{renderPass},
      frameBuffer{frameBuffer} {
    buffers.resize(MAX_FRAMES_IN_FLIGHT);
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool.GetCommandPool();
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = static_cast<uint32_t>(buffers.size());

    if (vkAllocateCommandBuffers(device.GetDevice(), &allocInfo, buffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate command buffers");
    }

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        commandBuffers.push_back(std::make_unique<CommandBuffer>(device, commandPool, swapChain, renderPass,
                                                                 frameBuffer, pipeline, buffers[i]));
        imageAvailableSemaphores.push_back(std::make_unique<Sync::Semaphore>(device));
        renderFinishedSemaphores.push_back(std::make_unique<Sync::Semaphore>(device));
        inFlightFences.push_back(std::make_unique<Sync::Fence>(device));
    }
}

void Renderer::Draw() {
    inFlightFences[currentFrame]->Wait();

    auto wait = imageAvailableSemaphores[currentFrame]->GetSemaphore();
    auto signal = renderFinishedSemaphores[currentFrame]->GetSemaphore();
    auto fence = inFlightFences[currentFrame]->GetFence();

    auto nextImageResult = swapChain->NextImage(wait);
    VkResult result = nextImageResult.result;
    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        recreate();
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("Failed to acquire swap chain image");
    }

    inFlightFences[currentFrame]->Reset();

    uint32_t imageIndex = nextImageResult.image;

    commandBuffers[currentFrame]->Reset();
    commandBuffers[currentFrame]->Record(imageIndex);
    commandBuffers[currentFrame]->Submit(&wait, &signal, fence);

    present(&signal, &imageIndex);

    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

// TODO: rethink swapchain recreation.
// Maybe some class method to recreate instance in stack rather than recreate by pointer
void Renderer::recreate() {
    auto extent = window.GetExtent();
    while (extent.width == 0 || extent.height == 0) {
        extent = window.GetExtent();
        Events::WaitEvents();
    }

    Events::WaitIdle(device);
    frameBuffer.reset();
    swapChain.reset();

    swapChain.reset(new SwapChain(window, vulkan, physDevice, device));
    frameBuffer.reset(new FrameBuffer(device, swapChain, renderPass));
}

void Renderer::present(VkSemaphore* signal, uint32_t* imageIndices) {
    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signal;

    VkSwapchainKHR swapChains[] = {swapChain->GetSwapChain()};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = imageIndices;

    VkResult result = vkQueuePresentKHR(device.GetPresentQueue(), &presentInfo);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window.IsFramebufferResized()) {
        window.setFramebufferResized(false);
        recreate();
    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to present swap chain image");
    }
}

}   // namespace Ely