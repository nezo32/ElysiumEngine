#include "frame_buffers.hpp"

#include "dependencies.hpp"

namespace Ely {

FrameBuffers::FrameBuffers(ElysiumDependencies &deps) : deps{deps} {
    auto swapChainImageViews = deps.swapChain->GetImageViews();
    auto swapChainExtent = deps.swapChain->GetExtent();
    frameBuffers.resize(swapChainImageViews.size());

    for (size_t i = 0; i < swapChainImageViews.size(); i++) {
        VkImageView attachments[] = {swapChainImageViews[i]};

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = deps.renderPass->GetRenderPass();
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = swapChainExtent.width;
        framebufferInfo.height = swapChainExtent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(deps.device->GetDevice(), &framebufferInfo, nullptr, &frameBuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create framebuffer");
        }
    }
}

FrameBuffers::~FrameBuffers() {
    for (auto framebuffer : frameBuffers) {
        vkDestroyFramebuffer(deps.device->GetDevice(), framebuffer, nullptr);
    }
}

}   // namespace Ely