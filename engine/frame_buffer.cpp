#include "frame_buffer.hpp"

namespace Ely {

FrameBuffer::FrameBuffer(Device& d, std::unique_ptr<SwapChain>& swapChain, RenderPass& renderPass) : device{d} {
    auto swapChainImageViews = swapChain->GetImageViews();
    auto swapChainExtent = swapChain->GetExtent();
    frameBuffers.resize(swapChainImageViews.size());

    for (size_t i = 0; i < swapChainImageViews.size(); i++) {
        VkImageView attachments[] = {swapChainImageViews[i]};

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderPass.GetRenderPass();
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = swapChainExtent.width;
        framebufferInfo.height = swapChainExtent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(device.GetDevice(), &framebufferInfo, nullptr, &frameBuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create framebuffer");
        }
    }
}

FrameBuffer::~FrameBuffer() {
    for (auto framebuffer : frameBuffers) {
        vkDestroyFramebuffer(device.GetDevice(), framebuffer, nullptr);
    }
}

}   // namespace Ely