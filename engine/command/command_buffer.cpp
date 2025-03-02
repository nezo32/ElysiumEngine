#include "command_buffer.hpp"

#include "dependencies.hpp"

namespace Ely {

CommandBuffer::CommandBuffer(ElysiumDependencies& deps, VkCommandBuffer buffer) : deps{deps} {
    if (buffer) {
        commandBuffer = buffer;
        return;
    }
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = deps.commandPools[CommandPoolCore]->GetCommandPool();
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    if (vkAllocateCommandBuffers(deps.device->GetDevice(), &allocInfo, &commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
    }
}

void CommandBuffer::Reset(VkCommandBufferResetFlags flags) { vkResetCommandBuffer(commandBuffer, flags); }

void CommandBuffer::Submit(VkSemaphore* waitSemaphores, VkSemaphore* signalSemaphores, VkFence fence,
                           uint32_t waitSemaphoresCount, uint32_t signalSemaphoresCount) {
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = waitSemaphoresCount;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    submitInfo.signalSemaphoreCount = signalSemaphoresCount;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(deps.device->GetGraphicsQueue(), 1, &submitInfo, fence) != VK_SUCCESS) {
        throw std::runtime_error("Failed to submit draw command buffer");
    }
}

void CommandBuffer::BeginRecord() {
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0;                    // Optional
    beginInfo.pInheritanceInfo = nullptr;   // Optional

    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("Failed to begin recording command buffer!");
    }
}

void CommandBuffer::BeginRenderPass(uint32_t imageIndex) {
    auto extent = deps.swapChain->GetExtent();

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = deps.renderPass->GetRenderPass();
    renderPassInfo.framebuffer = deps.frameBuffers->GetFrameBuffers()[imageIndex];

    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = extent;

    VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(extent.width);
    viewport.height = static_cast<float>(extent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = extent;
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
}

void CommandBuffer::EndRenderPass() { vkCmdEndRenderPass(commandBuffer); }

void CommandBuffer::EndRecord() {
    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("Failed to record command buffer");
    }
}

}   // namespace Ely