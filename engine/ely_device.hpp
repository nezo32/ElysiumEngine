#pragma once

#include "ely_phys_device.hpp"
#include "ely_vulkan.hpp"
#include "external/ely_glfw.hpp"

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
};
}   // namespace Ely