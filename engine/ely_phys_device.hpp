#pragma once

#include "ely_vulkan.hpp"
#include "external/ely_glfw.hpp"

namespace Ely {
class ElyPhysDevice {
   private:
    ElyVulkan &elyVulkan;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

    void pickPhysicalDevice();
    int rateDeviceSuitability(VkPhysicalDevice device);

   public:
    ElyPhysDevice(ElyVulkan &vulkan);
    ~ElyPhysDevice();

    ElyPhysDevice(const ElyPhysDevice &) = delete;
    ElyPhysDevice &operator=(const ElyPhysDevice &) = delete;
    ElyPhysDevice(ElyPhysDevice &&) = delete;
    ElyPhysDevice &operator=(ElyPhysDevice &&) = delete;

    VkPhysicalDevice GetPhysicalDevice() { return physicalDevice; }
};
}   // namespace Ely