#pragma once

#include "ely_swap_chain.hpp"
#include "ely_vulkan.hpp"
#include "ely_window.hpp"
#include "external/ely_glfw.hpp"

namespace Ely {
class PhysDevice {
   private:
    Vulkan &elyVulkan;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

    void pickPhysicalDevice();
    uint32_t rateDeviceSuitability(VkPhysicalDevice device);
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);

   public:
    PhysDevice(Vulkan &vulkan);
    ~PhysDevice();

    PhysDevice(const PhysDevice &) = delete;
    PhysDevice &operator=(const PhysDevice &) = delete;
    PhysDevice(PhysDevice &&) = delete;
    PhysDevice &operator=(PhysDevice &&) = delete;

    VkPhysicalDevice GetPhysicalDevice() { return physicalDevice; }
};
}   // namespace Ely