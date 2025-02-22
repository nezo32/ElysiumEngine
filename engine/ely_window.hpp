#pragma once

#include <iostream>

#include "external/ely_glfw.hpp"

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

    uint32_t GetWidth() { return static_cast<uint32_t>(width); }
    uint32_t GetHeight() { return static_cast<uint32_t>(height); }
    VkExtent2D GetExtent() { return {GetWidth(), GetHeight()}; }
    void CreateWindowSurface(VkInstance instance, VkSurfaceKHR *surface);
    bool ShouldClose() { return glfwWindowShouldClose(window); }
    void Close() { return glfwSetWindowShouldClose(window, true); }
};
}   // namespace Ely
