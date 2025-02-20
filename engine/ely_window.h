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

  void CreateWindowSurface(VkInstance instance, VkSurfaceKHR *surface);
  bool ShouldClose() { return glfwWindowShouldClose(window); }
  void Close() { return glfwSetWindowShouldClose(window, true); }
};
} // namespace Ely
