#include "renderer.hpp"

#include "dependencies.hpp"

namespace Ely {

Renderer::Renderer(ElysiumDependencies& deps) : deps{deps} {
    for (size_t i = 0; i < deps.consts.MAX_FRAMES_IN_FLIGHT; i++) {
        commandBuffers.push_back(std::make_unique<CommandBuffer>(deps));
        imageAvailableSemaphores.push_back(std::make_unique<Sync::Semaphore>(*deps.device));
        renderFinishedSemaphores.push_back(std::make_unique<Sync::Semaphore>(*deps.device));
        inFlightFences.push_back(std::make_unique<Sync::Fence>(*deps.device));
    }
}

void Renderer::PollEvents() { glfwPollEvents(); }

void Renderer::WaitEvents() { glfwWaitEvents(); }

void Renderer::WaitIdle() { vkDeviceWaitIdle(deps.device->GetDevice()); }

void Renderer::Draw() {
    inFlightFences[currentFrame]->Wait();

    auto wait = imageAvailableSemaphores[currentFrame]->GetSemaphore();
    auto signal = renderFinishedSemaphores[currentFrame]->GetSemaphore();
    auto fence = inFlightFences[currentFrame]->GetFence();

    auto nextImageResult = deps.swapChain->NextImage(wait);
    VkResult result = nextImageResult.result;
    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        recreate();
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("Failed to acquire swap chain image");
    }

    /* updateUniform(currentFrame); */

    inFlightFences[currentFrame]->Reset();

    uint32_t imageIndex = nextImageResult.image;

    auto commandBuffer = commandBuffers[currentFrame].get();

    commandBuffer->Reset();
    commandBuffer->BeginRecord();
    commandBuffer->BeginRenderPass(imageIndex);

    deps.pipelines[PipelineCore]->Bind(commandBuffer->GetCommandBuffer());

    auto buff = commandBuffer->GetCommandBuffer();
    for (auto& mesh : deps.meshes) {
        VkBuffer vertexBuffers[] = {mesh->vertexBuffer->GetVertexBuffer()};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(buff, 0, 1, vertexBuffers, offsets);

        vkCmdBindIndexBuffer(buff, mesh->indexBuffer->GetIndexBuffer(), 0, VK_INDEX_TYPE_UINT32);

        /* vkCmdBindDescriptorSets(buff, VK_PIPELINE_BIND_POINT_GRAPHICS, deps.pipelineLayout->GetPipelineLayout(), 0,
           1, &deps.descriptorSets->GetDescriptorSets()[currentFrame], 0, nullptr); */

        vkCmdDrawIndexed(buff, static_cast<uint32_t>(mesh->indexBuffer->GetIndicesCount()), 1, 0, 0, 0);
    }

    commandBuffer->EndRenderPass();
    commandBuffer->EndRecord();
    commandBuffer->Submit(&wait, &signal, fence);

    present(&signal, &imageIndex);

    currentFrame = (currentFrame + 1) % deps.consts.MAX_FRAMES_IN_FLIGHT;
}

void Renderer::updateUniform(size_t frameIndex) {
    SpaceTransformObject sto{};
    sto.model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    sto.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    sto.projection = glm::perspective(glm::radians(45.0f), deps.swapChain->GetExtentRatio(), 0.1f, 10.0f);
    sto.projection[1][1] *= -1;

    deps.uniformBuffers->Update(frameIndex, &sto);
}

void Renderer::recreate() {
    auto extent = deps.window->GetExtent();
    while (extent.width == 0 || extent.height == 0) {
        extent = deps.window->GetExtent();
        WaitEvents();
    }

    WaitIdle();
    delete deps.frameBuffers;
    delete deps.swapChain;

    deps.swapChain = new SwapChain(deps);
    deps.frameBuffers = new FrameBuffers(deps);
}

void Renderer::present(VkSemaphore* signal, uint32_t* imageIndices) {
    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signal;

    VkSwapchainKHR swapChains[] = {deps.swapChain->GetSwapChain()};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = imageIndices;

    VkResult result = vkQueuePresentKHR(deps.device->GetPresentQueue(), &presentInfo);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || deps.window->IsFramebufferResized()) {
        deps.window->SetFramebufferResized(false);
        recreate();
    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to present swap chain image");
    }
}

}   // namespace Ely