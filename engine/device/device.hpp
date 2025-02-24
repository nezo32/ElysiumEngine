#pragma once

#include "external/glfw.hpp"
#include "phys_device.hpp"
#include "vulkan.hpp"

namespace Ely {
class PhysDevice;
class Device {
   private:
    VkDevice device;
    VkQueue graphicsQueue;
    VkQueue presentQueue;

   public:
    Device(Vulkan &Vulkan, PhysDevice &physDevice);
    ~Device();

    Device(const Device &) = delete;
    Device &operator=(const Device &) = delete;
    Device(Device &&) = delete;
    Device &operator=(Device &&) = delete;

    VkDevice GetDevice() { return device; }
    VkQueue GetGraphicsQueue() { return graphicsQueue; }
    VkQueue GetPresentQueue() { return presentQueue; }
};
}   // namespace Ely