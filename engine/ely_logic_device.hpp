#pragma once

#include "ely_phys_device.hpp"
#include "external/ely_glfw.hpp"

namespace Ely {
class ElyLogicDevice {
   private:
    ElyPhysDevice &elyPhysDevice;
    VkDevice device;
    VkQueue graphicsQueue;

   public:
    ElyLogicDevice(ElyPhysDevice &physDevice);
    ~ElyLogicDevice();

    ElyLogicDevice(const ElyLogicDevice &) = delete;
    ElyLogicDevice &operator=(const ElyLogicDevice &) = delete;
    ElyLogicDevice(ElyLogicDevice &&) = delete;
    ElyLogicDevice &operator=(ElyLogicDevice &&) = delete;
};
}   // namespace Ely