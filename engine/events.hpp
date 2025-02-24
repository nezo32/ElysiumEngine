#pragma once

#include "device/device.hpp"
#include "external/glfw.hpp"

namespace Ely {

class Events {
   private:
   public:
    static void PollEvents();
    static void WaitEvents();
    static void WaitIdle(Device& device);
};

}   // namespace Ely