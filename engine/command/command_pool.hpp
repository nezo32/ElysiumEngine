#pragma once

#include "external/glfw.hpp"

namespace Ely {

/* TODO
The buffer copy command requires a queue family that supports transfer operations, which is indicated using
VK_QUEUE_TRANSFER_BIT. The good news is that any queue family with VK_QUEUE_GRAPHICS_BIT or VK_QUEUE_COMPUTE_BIT
capabilities already implicitly support VK_QUEUE_TRANSFER_BIT operations. The implementation is not required to
explicitly list it in queueFlags in those cases.

If you like a challenge, then you can still try to use a different queue family specifically for transfer operations. It
will require you to make the following modifications to your program:

Modify QueueFamilyIndices and findQueueFamilies to explicitly look for a queue family with the VK_QUEUE_TRANSFER_BIT
bit, but not the VK_QUEUE_GRAPHICS_BIT.

Modify createLogicalDevice to request a handle to the transfer queue

Create a second command pool for command buffers that are submitted on the transfer queue family

Change the sharingMode of resources to be VK_SHARING_MODE_CONCURRENT and specify both the graphics and transfer queue
families

Submit any transfer commands like vkCmdCopyBuffer (which we’ll be using in this chapter) to the transfer queue instead
of the graphics queue

It’s a bit of work, but it’ll teach you a lot about how resources are shared between queue families.
*/

struct ElysiumDependencies;

class CommandPool {
   private:
    ElysiumDependencies& deps;
    VkCommandPool pool;

   public:
    CommandPool(ElysiumDependencies& deps);
    ~CommandPool();

    VkCommandPool GetCommandPool() { return pool; }
};
}   // namespace Ely