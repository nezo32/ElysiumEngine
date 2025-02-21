#pragma once

#include "external/ely_glfw.hpp"

namespace Ely {

class ElyEvents {
   private:
   public:
    static void PollEvents() { glfwPollEvents(); };
};

}   // namespace Ely