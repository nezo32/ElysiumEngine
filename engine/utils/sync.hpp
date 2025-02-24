#pragma once

#include "device/device.hpp"
#include "external/glfw.hpp"

namespace Ely::Sync {
class Semaphore {
   private:
    Device& device;
    VkSemaphore semaphore;

   public:
    Semaphore(Device& d);
    ~Semaphore();

    VkSemaphore GetSemaphore() { return semaphore; }
};

class Fence {
   private:
    Device& device;
    VkFence fence;

   public:
    Fence(Device& d);
    ~Fence();

    void Wait(uint64_t timeout = UINT64_MAX);
    void Reset();

    static void WaitMultiple(Device& device, VkFence* fences, uint32_t fencesCount, VkBool32 waitAll = VK_TRUE,
                             uint64_t timeout = UINT64_MAX);
    static void ResetMultiple(Device& device, VkFence* fences, uint32_t fencesCount);

    VkFence GetFence() { return fence; }
};
}   // namespace Ely::Sync