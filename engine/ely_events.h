#pragma once

#include "external/ely_glfw.h"

namespace Ely {

class ElyEvents {
private:
public:
  static void PollEvents() { glfwPollEvents(); };
};

} // namespace Ely