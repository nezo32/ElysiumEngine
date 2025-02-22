#pragma once

#include "ely_phys_device.hpp"
#include "external/ely_glfw.hpp"

namespace Ely {
class ElyDevice {
   private:
    VkDevice device;
    VkQueue graphicsQueue;

   public:
    ElyDevice(ElyPhysDevice &physDevice);
    ~ElyDevice();

    ElyDevice(const ElyDevice &) = delete;
    ElyDevice &operator=(const ElyDevice &) = delete;
    ElyDevice(ElyDevice &&) = delete;
    ElyDevice &operator=(ElyDevice &&) = delete;

    VkDevice GetDevice() { return device; }
};
}   // namespace Ely