#pragma once

#include "external/glfw.hpp"

namespace Ely {

struct ElysiumDependencies;
class PhysDevice;
class Device {
   private:
    VkDevice device;
    VkQueue graphicsQueue;
    VkQueue presentQueue;

   public:
    Device(ElysiumDependencies &deps);
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