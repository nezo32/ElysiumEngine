#pragma once

#include "device/device.hpp"
#include "device/phys_device.hpp"
#include "external/glfw.hpp"
#include "frame_buffer.hpp"
#include "render_pass.hpp"
#include "swap_chain.hpp"
#include "utils/queue_families.hpp"
#include "vulkan.hpp"

namespace Ely {

class CommandPool {
   private:
    Device& device;
    VkCommandPool commandPool;

   public:
    CommandPool(Vulkan& vulkan, PhysDevice& physDevice, Device& device);
    ~CommandPool();

    VkCommandPool GetCommandPool() { return commandPool; }
};
}   // namespace Ely