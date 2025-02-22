#pragma once

#include "external/ely_glfw.hpp"

namespace Ely {

class Events {
   private:
   public:
    static void PollEvents() { glfwPollEvents(); };
};

}   // namespace Ely