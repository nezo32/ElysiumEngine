#include "events.hpp"

namespace Ely {

void Events::PollEvents() { glfwPollEvents(); }

void Events::WaitEvents() { glfwWaitEvents(); }

void Events::WaitIdle(Device& device) { vkDeviceWaitIdle(device.GetDevice()); }

}   // namespace Ely
