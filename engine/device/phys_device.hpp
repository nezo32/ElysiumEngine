#pragma once

#include "external/glfw.hpp"

namespace Ely {

struct ElysiumDependencies;

class PhysDevice {
   private:
    ElysiumDependencies &deps;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkPhysicalDeviceMemoryProperties memProperties;

    void pickPhysicalDevice();
    uint32_t rateDeviceSuitability(VkPhysicalDevice device);
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);

   public:
    PhysDevice(ElysiumDependencies &deps);

    PhysDevice(const PhysDevice &) = delete;
    PhysDevice &operator=(const PhysDevice &) = delete;
    PhysDevice(PhysDevice &&) = delete;
    PhysDevice &operator=(PhysDevice &&) = delete;

    uint32_t FindMemoryType(uint32_t type, VkMemoryPropertyFlags properties);

    VkPhysicalDeviceMemoryProperties &GetMemoryProperties() { return memProperties; }
    VkPhysicalDevice GetPhysicalDevice() { return physicalDevice; }
};
}   // namespace Ely