#pragma once

#include "external/ely_glfw.h"
#include <iostream>

namespace Ely {
class ElyWindow {
private:
  GLFWwindow *window;
  int width;
  int height;

  void setHints();

public:
  ElyWindow(const int &width, const int &height, const char *title);
  ~ElyWindow();

  ElyWindow(const ElyWindow &) = delete;
  ElyWindow &operator=(const ElyWindow &) = delete;

  bool ShouldClose() { return glfwWindowShouldClose(window); }
};
} // namespace Ely
