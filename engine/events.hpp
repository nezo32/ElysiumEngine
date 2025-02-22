#pragma once

#include "external/glfw.hpp"

namespace Ely {

class Events {
   private:
   public:
    static void PollEvents() { glfwPollEvents(); };
};

}   // namespace Ely