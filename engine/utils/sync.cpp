#include "sync.hpp"

#include <stdexcept>

namespace Ely::Sync {

Semaphore::Semaphore(Device& d) : device{d} {
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    if (vkCreateSemaphore(device.GetDevice(), &semaphoreInfo, nullptr, &semaphore) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create semaphore");
    }
}

Semaphore::~Semaphore() { vkDestroySemaphore(device.GetDevice(), semaphore, nullptr); }

Fence::Fence(Device& d) : device{d} {
    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    if (vkCreateFence(device.GetDevice(), &fenceInfo, nullptr, &fence) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create fence");
    }
}

Fence::~Fence() { vkDestroyFence(device.GetDevice(), fence, nullptr); }

void Fence::Wait(uint64_t timeout) { vkWaitForFences(device.GetDevice(), 1, &fence, VK_TRUE, timeout); }

void Fence::Reset() { vkResetFences(device.GetDevice(), 1, &fence); }

void Fence::WaitMultiple(Device& device, VkFence* fences, uint32_t fencesCount, VkBool32 waitAll, uint64_t timeout) {
    vkWaitForFences(device.GetDevice(), fencesCount, fences, waitAll, timeout);
}

void Fence::ResetMultiple(Device& device, VkFence* fences, uint32_t fencesCount) {
    vkResetFences(device.GetDevice(), fencesCount, fences);
}

}   // namespace Ely::Sync